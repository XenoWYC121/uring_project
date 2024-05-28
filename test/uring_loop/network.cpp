//
// Created by 12992 on 24-5-28.
//
#include "uring_loop/uring_loop.h"

#include <gtest/gtest.h>

#include <iostream>

#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
using namespace uring_project;

coroutine::task<int> net_test(uring::uring_loop &loop)
{
    int acc_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (acc_fd < 0)
    {
        perror("socket error");
        co_return -1;
    }
    sockaddr_in addr{};
    addr.sin_port = htons(25444);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int res = ::bind(acc_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
    if (res < 0)
    {
        perror("bind error");
        co_return -1;
    }
    res = listen(acc_fd, 128);
    if (res < 0)
    {
        perror("listen error");
        co_return -1;
    }
    sockaddr_in endpoint_addr{};
    socklen_t endpoint_len{};
    int conn_fd = co_await loop.async_accept(acc_fd, endpoint_addr, endpoint_len);
    if (conn_fd < 0)
    {
        perror("accept error");
        co_return -1;
    }
    res = co_await loop.async_write(conn_fd, "hello world!", 12);
    if (res < 0)
    {
        perror("write error");
        co_return -1;
    }
    array<char, 128> buf{};
    res = co_await loop.async_read(conn_fd, &buf[0], 128);
    cout.write(buf.data(), res);
    close(conn_fd);
    close(acc_fd);
    co_return 0;
}

TEST(uring_network, 1)
{
    uring::uring_loop loop(128);
    loop.new_task(net_test(loop));
    loop.run();
}