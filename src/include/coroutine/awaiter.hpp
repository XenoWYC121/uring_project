//
// Created by 12992 on 24-5-26.
//

#ifndef URING_PROJECT_AWAITER_HPP
#define URING_PROJECT_AWAITER_HPP

#include "promise.hpp"

#include <coroutine>
#include <iostream>
#include <utility>

#include <arpa/inet.h>


namespace uring_project::uring
{
    class uring_loop;
}

namespace uring_project::coroutine
{

    template<typename T>
    class task_awaiter
    {
        using promise_type = promise<T>;
        using handler_type = std::coroutine_handle<promise_type>;
    public:
        explicit task_awaiter(handler_type handler) : m_handler(handler) {}

        bool await_ready() const noexcept { return false; }


        std::coroutine_handle<promise_type> await_suspend(std::coroutine_handle<> coroutine) const noexcept
        {
            //mCoroutine.promise().mPrevious = coroutine;
            this->m_handler.promise().m_prev_handler = coroutine;
            return this->m_handler;
        }

        T await_resume() const
        {
            return this->m_handler.promise().get_value();
        }

    private:
        handler_type m_handler;
    };

    class exit_awaiter
    {
    public:
        bool await_ready() const noexcept { return false; }


        void await_suspend(std::coroutine_handle<> coroutine) const noexcept {}

        void await_resume() const {}
    };

    class async_open_at_awaiter
    {
        using promise_type = promise<int>;
        using handler_type = std::coroutine_handle<promise_type>;
    public:
        async_open_at_awaiter(uring::uring_loop& loop, int dfd, std::string file_path, int oflag, int mode)
                : m_loop(&loop), m_dfd(dfd), m_file_path(std::move(file_path)), m_oflag(oflag), m_mode(mode) {}

        bool await_ready() const noexcept { return false; }

        int await_resume() const { return this->m_handler.promise().get_value(); }

        void await_suspend(handler_type coroutine) noexcept;

    private:
        uring::uring_loop* m_loop{};
        int m_dfd{};
        std::string m_file_path{};
        int m_oflag{};
        int m_mode{};
        handler_type m_handler{};
    };

    class async_read_awaiter
    {
        using promise_type = promise<int>;
        using handler_type = std::coroutine_handle<promise_type>;
    public:
        async_read_awaiter(uring::uring_loop& loop, int fd, char* buffer, size_t read_size)
                : m_loop(&loop), m_fd(fd), m_buffer(buffer), m_read_size(read_size) {}

        bool await_ready() const noexcept { return false; }

        int await_resume() const { return this->m_handler.promise().get_value(); }

        void await_suspend(handler_type coroutine) noexcept;

    private:
        uring::uring_loop* m_loop{};
        int m_fd{};
        char* m_buffer{nullptr};
        size_t m_read_size{0};
        handler_type m_handler{};
    };

    class async_write_awaiter
    {
        using promise_type = promise<int>;
        using handler_type = std::coroutine_handle<promise_type>;
    public:
        async_write_awaiter(uring::uring_loop& loop, int fd, const char* buffer, size_t write_size)
                : m_loop(&loop), m_fd(fd), m_buffer(buffer), m_write_size(write_size) {}

        bool await_ready() const noexcept { return false; }

        int await_resume() const { return this->m_handler.promise().get_value(); }

        void await_suspend(handler_type coroutine) noexcept;

    private:
        uring::uring_loop* m_loop{};
        int m_fd{};
        const char* m_buffer{nullptr};
        size_t m_write_size{0};
        handler_type m_handler{};
    };

    class async_accept_awaiter
    {
        using promise_type = promise<int>;
        using handler_type = std::coroutine_handle<promise_type>;
    public:
        async_accept_awaiter(uring::uring_loop& loop,
                             int accept_fd,
                             sockaddr_in& sock_addr,
                             socklen_t& socklen)
                : m_loop(&loop), m_accept_fd(accept_fd),
                  m_sock_addr(&sock_addr), m_socklen(&socklen) {}

        bool await_ready() const noexcept { return false; }

        int await_resume() const { return this->m_handler.promise().get_value(); }

        void await_suspend(handler_type coroutine) noexcept;

    private:
        uring::uring_loop* m_loop{};
        int m_accept_fd{};
        sockaddr_in* m_sock_addr{};
        socklen_t* m_socklen{};
        handler_type m_handler;
    };
}

#endif //URING_PROJECT_AWAITER_HPP
