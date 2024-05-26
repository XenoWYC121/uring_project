//
// Created by 12992 on 24-5-25.
//
#include "coroutine/task.hpp"

#include <gtest/gtest.h>

#include <iostream>


TEST(create_task, 1)
{
    using namespace uring_project::coroutine;
    auto value = []() -> task<void>
    {
        std::cout << "run in coroutine" << std::endl;
        co_return;
    };
    auto c = value();
    c.resume();
}

TEST(create_task, 2)
{
    using namespace uring_project::coroutine;
    auto value = []() -> task<void>
    {
        std::cout << "run in coroutine" << std::endl;
        co_return;
    };
    auto task1 = value();
    auto value2 = [&]() -> task<void>
    {
        std::cout << "in coroutine 2" << std::endl;
        co_await task1;
        std::cout << "in coroutine 2" << std::endl;
    };
    auto task2 = value2();
    task2.resume();
}


TEST(create_task, 3)
{
    using namespace uring_project::coroutine;

    auto value2 = [&]() -> task<void>
    {
        std::cout << "in coroutine 2" << std::endl;
        co_await exit_awaiter();
        std::cout << "in coroutine 2" << std::endl;
    };
    auto task2 = value2();
    task2.resume();
    task2.resume();
}


TEST(create_task, 4)
{
    using namespace uring_project::coroutine;
    auto value = []() -> task<int>
    {
        std::cout << "run in coroutine" << std::endl;
        throw std::runtime_error("hello");
        co_return 20;
    };
    auto task1 = value();
    auto value2 = [&]() -> task<void>
    {
        std::cout << "in coroutine 2" << std::endl;
        try
        {
            int v = co_await task1;
            std::cout << "get : " << v << " from task1" << std::endl;
        }
        catch (const std::runtime_error& error)
        {
            std::cerr << error.what() << std::endl;
        }
        std::cout << "in coroutine 2" << std::endl;
    };
    auto task2 = value2();
    task2.resume();
    ASSERT_NO_THROW(task2.get_value());
}

TEST(create_task, 5)
{
    using namespace uring_project::coroutine;
    auto value = []() -> task<int>
    {
        std::cout << "run in coroutine" << std::endl;
        throw std::runtime_error("hello");
        co_return 20;
    };
    auto task1 = value();
    auto value2 = [&]() -> task<void>
    {
        std::cout << "in coroutine 2" << std::endl;
        int v = co_await task1;
        std::cout << "get : " << v << " from task1" << std::endl;
        std::cout << "in coroutine 2" << std::endl;
    };
    auto task2 = value2();
    task2.resume();
    ASSERT_THROW(task2.get_value(), std::runtime_error);
}