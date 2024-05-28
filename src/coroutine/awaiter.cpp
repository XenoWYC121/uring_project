//
// Created by 12992 on 24-5-26.
//
#include "coroutine/awaiter.hpp"
#include "uring_loop/uring_loop.h"

namespace uring_project::coroutine
{

    void async_open_at_awaiter::await_suspend(handler_type coroutine) noexcept
    {
        this->m_handler = coroutine;
        this->m_loop->new_async_op(
                [this](io_uring_sqe* entry) -> void
                {
                    io_uring_prep_openat(entry, this->m_dfd, this->m_file_path.c_str(),
                                         this->m_oflag, this->m_mode);
                    io_uring_sqe_set_data64(entry, reinterpret_cast<uint64_t>(&this->m_handler));
                });
    }

    void async_read_awaiter::await_suspend(std::coroutine_handle<async_read_awaiter::promise_type> coroutine) noexcept
    {
        this->m_handler = coroutine;
        this->m_loop->new_async_op(
                [this](io_uring_sqe* entry) -> void
                {
                    io_uring_prep_read(entry, this->m_fd, this->m_buffer, this->m_read_size, 0);
                    io_uring_sqe_set_data64(entry, reinterpret_cast<uint64_t>(&this->m_handler));
                });
    }

    void async_write_awaiter::await_suspend(async_write_awaiter::handler_type coroutine) noexcept
    {
        this->m_handler = coroutine;
        this->m_loop->new_async_op(
                [this](io_uring_sqe* entry) -> void
                {
                    io_uring_prep_write(entry, this->m_fd, this->m_buffer, this->m_write_size, 0);
                    io_uring_sqe_set_data64(entry, reinterpret_cast<uint64_t>(&this->m_handler));
                });
    }

    void async_accept_awaiter::await_suspend(async_accept_awaiter::handler_type coroutine) noexcept
    {
        this->m_handler = coroutine;
        this->m_loop->new_async_op(
                [this](io_uring_sqe* entry)
                {
                    io_uring_prep_accept(entry, this->m_accept_fd,
                                         reinterpret_cast<sockaddr*>(this->m_sock_addr),
                                         this->m_socklen, 0);
                    io_uring_sqe_set_data64(entry, reinterpret_cast<uint64_t>(&this->m_handler));
                });
    }
}