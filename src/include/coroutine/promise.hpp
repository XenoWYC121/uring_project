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
    class prev_awaiter
    {
        using handler_type = std::coroutine_handle<>;
    public:
        prev_awaiter() = default;

        explicit prev_awaiter(handler_type prev) : m_prev(prev) {}

        ~prev_awaiter() = default;


        bool await_ready() noexcept { return !(this->m_prev && !this->m_prev.done()); }


        [[nodiscard]] std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine) const noexcept
        {
            if (this->m_prev && !this->m_prev.done())
            {
                return this->m_prev;
            }
            return std::noop_coroutine();
        }

        void await_resume() const noexcept {}

    private:
        handler_type m_prev{};
    };

    struct base_promise
    {
        std::suspend_always initial_suspend() { return {}; }

        prev_awaiter final_suspend() noexcept { return prev_awaiter(this->m_perv_handler); }

        void unhandled_exception()
        {
            this->m_exception = std::current_exception();
        }

        const std::coroutine_handle<>& get_perv_handler() const
        {
            return m_perv_handler;
        }

        void set_perv_handler(const std::coroutine_handle<>& mPervHandler)
        {
            m_perv_handler = mPervHandler;
        }

        std::exception_ptr get_exception() { return this->m_exception; }

    protected:
        std::exception_ptr m_exception;
    private:
        std::coroutine_handle<> m_perv_handler;
    };

    template<typename T>
    struct promise
            : public base_promise
    {
    public:
        T get_value() const
        {
            if (this->m_exception)
            {
                std::rethrow_exception(this->m_exception);
            }
            return this->value;
        }

        void set_value(T t)
        {
            promise::value = std::move(t);
        }

        std::coroutine_handle<promise<T>> get_return_object()
        {
            return std::coroutine_handle<promise<T>>::from_promise(*this);
        }

        void return_value(const T& v)
        {
            value = v;
        }

    private:
        T value;
        std::coroutine_handle<promise<T>> m_promise;
    };

    template<>
    struct promise<void>
            : public base_promise
    {
    public:
        std::coroutine_handle<promise<void>> get_return_object()
        {
            return std::coroutine_handle<promise<void>>::from_promise(*this);
        }

        void return_void() {}

        void get_value()
        {
            if (this->m_exception)
            {
                std::rethrow_exception(this->m_exception);
            }
        }

    private:
        std::coroutine_handle<promise<void>> m_promise;
    };


}

#endif //URING_PROJECT_PROMISE_HPP
