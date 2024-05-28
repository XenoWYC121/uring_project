//
// Created by 12992 on 24-5-25.
//

#ifndef URING_PROJECT_PROMISE_HPP
#define URING_PROJECT_PROMISE_HPP

#include <utility>
#include <coroutine>
#include <exception>

namespace uring_project::coroutine
{
    struct await_prev
    {
        std::coroutine_handle<> prev;

        bool await_ready() const noexcept { return false; }

        ///
        /// \param handle 传入的参数为调用协程
        /// \return 返回被调用协程
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> handle) noexcept
        {
            if (prev && !prev.done())
            {
                return prev;
            }
            return std::noop_coroutine();
        }


        void await_resume() const noexcept {}
    };

    template<typename T>
    struct promise
    {
        std::suspend_always initial_suspend() { return {}; }

        await_prev final_suspend() noexcept { return {.prev = this->m_prev_handler}; }

        std::coroutine_handle<promise> get_return_object()
        {
            return std::coroutine_handle<promise>::from_promise(*this);
        }

        await_prev yield_value(int value)
        {
            this->m_value = value;
            return {.prev = m_prev_handler};
        }

        void unhandled_exception() noexcept {}

        //std::suspend_never return_void() { return {}; }

        std::coroutine_handle<> m_prev_handler;

        void set_value(const T &v) { this->m_value = v; }

        T get_value() const { return this->m_value; }

        std::suspend_never return_value(const T &value)
        {
            this->set_value(value);
            return {};
        }
        T m_value;
        unsigned long long key;
    };


}

#endif //URING_PROJECT_PROMISE_HPP
