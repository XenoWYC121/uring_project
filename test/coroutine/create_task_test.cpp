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
