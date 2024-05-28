//
// Created by 12992 on 24-5-26.
//

#include "uring_loop/uring_loop.h"
#include "coroutine/promise.hpp"
#include "coroutine/awaiter.hpp"
#include <coroutine>
#include <utility>

namespace uring_project::uring
{
    uring_loop::uring_loop(size_t size)
    {
        if (const auto res = io_uring_queue_init(size, &this->m_ring, 0); res < 0)
        {
            throw std::system_error(errno, std::generic_category());
        }
    }

    uring_loop::~uring_loop()
    {
        io_uring_queue_exit(&this->m_ring);
    }

    void uring_loop::run()
    {
        while (event_counter)
        {
            io_uring_cqe* cqe{};
            unsigned head;
            unsigned counter = 0;
            io_uring_wait_cqe(&this->m_ring, &cqe);
            io_uring_for_each_cqe(&this->m_ring, head, cqe)
            {
                auto ptr = reinterpret_cast<std::coroutine_handle<coroutine::promise<int> >*>(io_uring_cqe_get_data64(
                        cqe));
                if (ptr == nullptr)
                {
                    throw std::runtime_error("unknown error");
                }
                ptr->promise().set_value(cqe->res);
                ptr->resume();
                if (ptr->done())
                {
                    auto key = ptr->promise().key;
                    this->task_map.erase(key);
                }
                counter++;
                this->event_counter--;
            }
            io_uring_cq_advance(&this->m_ring, counter);
        }
    }

    void uring_loop::new_async_op(const std::function<void(io_uring_sqe*)>& op)
    {
        const auto sqe = io_uring_get_sqe(&this->m_ring);
        if (sqe == nullptr)
        {
            throw std::bad_alloc{};
        }
        op(sqe);
        int res = io_uring_submit(&this->m_ring);
        if (res < 0)
        {
            throw std::system_error(errno, std::generic_category());
        }
        ++this->event_counter;
    }

    void uring_loop::new_task(coroutine::task<int>&& task1)
    {
        const auto key = this->task_marker.load();
        ++this->task_marker;
        task1.set_key(key);
        this->task_map.emplace(key, std::move(task1));
        auto& task_ref = this->task_map.find(key)->second;
        task_ref.resume();
        if (!task_ref)
        {
            // 如果失效
            this->task_map.erase(key);
        }
    }

    uring_loop::uring_loop(uring_loop&& obj) noexcept
            : m_ring(std::exchange(obj.m_ring, {}))
    {
    }

    uring_loop& uring_loop::operator=(uring_loop&& obj) noexcept
    {
        auto move_temp = std::move(obj);
        std::swap(*this, move_temp);
        return *this;
    }

    coroutine::async_open_at_awaiter uring_loop::async_open_at(int dfd, const std::string& file, int oflags, int modes)
    {
        return {*this, dfd, file, oflags, modes};
    }

    coroutine::async_write_awaiter uring_loop::async_write(int fd, const char* buffer, size_t size)
    {
        return {*this, fd, buffer, size};
    }

    coroutine::async_read_awaiter uring_loop::async_read(int fd, char* buffer, size_t size)
    {
        return {*this, fd, buffer, size};
    }
}
