//
// Created by 12992 on 24-5-25.
//

#ifndef URING_PROJECT_TASK_HPP
#define URING_PROJECT_TASK_HPP

#include "promise.hpp"
#include "awaiter.hpp"

namespace uring_project::coroutine
{
    template<typename T, typename P = promise<T> >
    class task
    {
    public:
        using promise_type = P;
        using handler_type = std::coroutine_handle<promise_type>;

    public:
        task(handler_type handler) : m_handler(handler) {}

        task(task &&t) noexcept;

        task(const task &) = delete;

        task &operator=(const task &t) = delete;

        task &operator=(task &&t) noexcept;

        ~task();

        void set_key(unsigned long long value) { this->m_handler.promise().key = value; }

        explicit operator bool() const noexcept
        {
            return this->m_handler && !this->m_handler.done();
        }

        void resume()
        {
            if (this->m_handler && !this->m_handler.done())
            {
                this->m_handler.resume();
            }
        }

        T get_value() const
        {
            return this->m_handler.promise().get_value();
        }

    public:
        //        //在别人的协程中，调用你所需要的动作
        //        bool await_ready() const noexcept { return false; }
        //
        //        ///
        //        /// \param handle 传入的参数为调用协程
        //        /// \return 返回被调用协程
        //        std::coroutine_handle<> await_suspend(std::coroutine_handle<> handle)
        //        {
        //            if (this->m_handler && !this->m_handler.done())
        //            {
        //                this->m_handler.promise().m_prev_handler = handle;
        //                return this->m_handler;
        //            }
        //            return std::noop_coroutine();
        //        }
        //
        //        int await_resume() const noexcept { return 10; }

        auto operator
        co_await()
        {
            return task_awaiter<T>(this->m_handler);
        }

    private:
        handler_type m_handler;
    };

    template<typename T, typename P>
    task<T, P>::~task()
    {
        if (this->m_handler)
        {
            this->m_handler.destroy();
        }
    }

    template<typename T, typename P>
    task<T, P> &task<T, P>::operator=(task &&t) noexcept
    {
        auto temp_move = std::move(t);
        std::swap(*this, temp_move);
        return *this;
    }

    template<typename T, typename P>
    task<T, P>::task(task &&t) noexcept
        : m_handler(std::exchange(t.m_handler, {}))
    {
    }
} // coroutine
// uring_project

#endif //URING_PROJECT_TASK_HPP
