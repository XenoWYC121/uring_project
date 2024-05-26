//
// Created by 12992 on 24-5-26.
//
#include <iostream>
#include "uring_loop/uring_loop.h"

using namespace std;

uring_project::coroutine::task<int> v1(uring_project::uring::uring_loop& loop)
{
    int dfd = open("/home/fsy/test", O_RDONLY);
    if (dfd == -1)
    {
        throw std::system_error(errno, std::generic_category());
    }
    int res = 0;
    //res = co_await loop.async_open_at(dfd, "hello.txt", O_CREAT | O_RDWR, 0644);
    cout << "get res" << res << endl;
    if (res <= 0)
    {

    }
    close(res);
    close(dfd);
    co_return 10;
}

int main()
{
    using namespace uring_project;
    uring::uring_loop loop(128);
    auto v = v1(loop);
    v.resume();
    //loop.run();
}