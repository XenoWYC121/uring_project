//
// Created by 12992 on 24-5-26.
//
#include <gtest/gtest.h>
#include <liburing.h>
#include <iostream>
#include "coroutine/task.hpp"
#include "uring_loop/uring_loop.h"

using namespace std;

TEST(uring_test, 1)
{
    io_uring ring{};
    io_uring_queue_exit(&ring);
}

uring_project::coroutine::task<int> v1(uring_project::uring::uring_loop &loop)
{
    int dfd = open("/home/fsy/test", O_RDONLY);
    if (dfd == -1)
    {
        throw std::system_error(errno, std::generic_category());
    }
    int res = 0;
    res = co_await loop.async_open_at(dfd, "hello.txt", O_CREAT | O_RDWR, 0644);
    cout << "get res" << res << endl;
    if (res <= 0)
    {

    }
    close(res);
    close(dfd);
    co_return 10;
}

TEST(uring_test, 2)
{
    using namespace uring_project;
    uring::uring_loop loop(128);
    auto v = v1(loop);
    v.resume();
    loop.run();
}

uring_project::coroutine::task<int> hello(uring_project::uring::uring_loop &loop)
{
    int dfd = open("/home/fsy/test", O_RDONLY);
    if (dfd == -1)
    {
        throw std::system_error(errno, std::generic_category());
    }
    int res = 0;
    res = co_await loop.async_open_at(dfd, "hello4.txt", O_CREAT | O_RDWR, 0644);
    cout << "get res" << res << endl;
    if (res <= 0)
    {
        close(res);
        close(dfd);
        co_return -1;
    }
    int fd = res;
    char buffer[20]{};
    res = co_await loop.async_read(fd, buffer, 20);
    if (res <= 0)
    {
        close(fd);
        close(dfd);
        co_return -1;
    }
    cout.write(buffer, res);
    co_return 0;
}

void call_in_func(uring_project::uring::uring_loop &loop)
{
    auto task = hello(loop);
    task.resume();
}

TEST(uring_test, 3)
{
    uring_project::uring::uring_loop loop(128);
    auto task = hello(loop);
    task.resume();
    loop.run();
}

TEST(uring_test, 4)
{
    uring_project::uring::uring_loop loop(128);
    call_in_func(loop);
    loop.run();
}