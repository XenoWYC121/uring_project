//
// Created by 12992 on 24-5-25.
//

#ifndef URING_PROJECT_TASK_HPP
#define URING_PROJECT_TASK_HPP

#include "promise.hpp"

namespace uring_project::coroutine
{

    template<typename T, typename P = promise<T>>
    class task
    {
    public:
        using promise_type = P;
        using handler_type = std::coroutine_handle<promise_type>;
    public:
        task(handler_type handler) : m_handler(handler) {}

        void resume()
        {
            if (this->m_handler && !this->m_handler.done())
            {
                this->m_handler.resume();
            }
        }

    private:
        handler_type m_handler;
    };

} // coroutine
// uring_project

#endif //URING_PROJECT_TASK_HPP
