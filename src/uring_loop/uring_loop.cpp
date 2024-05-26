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
        auto res = io_uring_queue_init(size, &this->m_ring, 0);
        if (res < 0)
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
        io_uring_cqe* cqe{};
        unsigned head;
        unsigned counter = 0;
        io_uring_for_each_cqe(&this->m_ring, head, cqe)
        {
            auto ptr = reinterpret_cast<std::coroutine_handle<coroutine::promise<int>>*>(io_uring_cqe_get_data64(cqe));
            if (ptr == nullptr)
            {
                throw std::runtime_error("unknown error");
            }
            std::coroutine_handle<coroutine::promise<int>> temp_obj = *ptr;
            delete ptr;
            temp_obj.promise().set_value(cqe->res);
            temp_obj.resume();
            counter++;
        }
        io_uring_cq_advance(&this->m_ring, counter);
    }

    void uring_loop::new_async_op(const std::function<void(io_uring_sqe*)>& op)
    {
        auto sqe = io_uring_get_sqe(&this->m_ring);
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
        return coroutine::async_open_at_awaiter(*this, dfd, file, oflags, modes);
    }
}