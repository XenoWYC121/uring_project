//
// Created by 12992 on 24-5-26.
//
#include <gtest/gtest.h>
#include <liburing.h>
#include <iostream>


using namespace std;

TEST(uring_test, 1)
{
    io_uring ring{};
    io_uring_queue_exit(&ring);
}