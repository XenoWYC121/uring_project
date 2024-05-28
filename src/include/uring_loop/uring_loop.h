//
// Created by 12992 on 24-5-26.
//

#ifndef URING_PROJECT_URING_LOOP_H
#define URING_PROJECT_URING_LOOP_H

#include "coroutine/task.hpp"

#include <unistd.h>
#include <liburing.h>
#include <atomic>
#include <functional>

namespace uring_project::coroutine
{
    class async_open_at_awaiter;
}

namespace uring_project::uring
{
    class uring_loop final
    {
    public:
        explicit uring_loop(size_t size);

        ~uring_loop();

        uring_loop(const uring_loop &) = delete;

        uring_loop(uring_loop &&obj) noexcept;

        uring_loop &operator=(const uring_loop &) = delete;

        uring_loop &operator=(uring_loop &&obj) noexcept;

    public:
        coroutine::async_open_at_awaiter async_open_at(int dfd, const std::string &file, int oflags, int modes);

        coroutine::async_read_awaiter async_read(int fd, char *buffer, size_t size);

        coroutine::async_write_awaiter async_write(int fd, const char *buffer, size_t size);

    public:
        void new_async_op(const std::function<void(io_uring_sqe *)> &op);

        void new_task(coroutine::task<int> &&task1);

        void run();

    private:
        io_uring m_ring{};
        std::atomic<int> event_counter;
        std::unordered_map<unsigned, coroutine::task<int> > task_map;
        std::atomic<unsigned long long> task_marker{0};
    };
} // uring
// uring_project

#endif //URING_PROJECT_URING_LOOP_H
