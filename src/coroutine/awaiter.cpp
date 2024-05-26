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
}