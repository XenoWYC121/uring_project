//
// Created by 12992 on 24-5-26.
//

#ifndef URING_PROJECT_URING_LOOP_H
#define URING_PROJECT_URING_LOOP_H

#include "coroutine/task.hpp"

#include <unistd.h>
#include <liburing.h>

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

        uring_loop(const uring_loop&) = delete;

        uring_loop(uring_loop&& obj) noexcept;

        uring_loop& operator=(const uring_loop&) = delete;

        uring_loop& operator=(uring_loop&& obj) noexcept;

    public:
        coroutine::task<int> async_open_at(int dfd, const std::string& file, int oflags, int modes);

    public:

        void new_async_op(const std::function<void(io_uring_sqe*)>& op);

        void run();

    private:
        io_uring m_ring{};
    };

} // uring
// uring_project

#endif //URING_PROJECT_URING_LOOP_H