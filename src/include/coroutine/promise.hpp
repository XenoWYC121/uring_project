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
    struct base_promise
    {
        std::suspend_always initial_suspend() { return {}; }

        std::suspend_never final_suspend() noexcept { return {}; }

        void unhandled_exception()
        {
            this->m_exception = std::current_exception();
        }

    private:
        std::exception_ptr m_exception;
    };

    template<typename T>
    struct promise
            : public base_promise
    {
    public:
        T get_value() const
        {
            return value;
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

    private:
        std::coroutine_handle<promise<void>> m_promise;
    };


}

#endif //URING_PROJECT_PROMISE_HPP
