//
// Created by 12992 on 24-5-26.
//

#ifndef URING_PROJECT_AWAITER_HPP
#define URING_PROJECT_AWAITER_HPP

#include "promise.hpp"

#include <coroutine>
#include <iostream>

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
            this->m_handler.promise().set_perv_handler(coroutine);
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

}

#endif //URING_PROJECT_AWAITER_HPP
