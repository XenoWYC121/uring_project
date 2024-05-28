//
// Created by 12992 on 24-5-28.
//

#ifndef URING_PROJECT_INFO_BLOCK_H
#define URING_PROJECT_INFO_BLOCK_H

#include "coroutine/task.hpp"

namespace uring_project::uring
{
    class info_block
    {
        using handler_type = coroutine::task<int>::handler_type;
    public:

        static uint64_t create_info_block(handler_type &handler, int fd);

        static info_block parse(uint64_t v);

        handler_type *get_handler_ptr() const;

        int get_target_fd() const;

    private:
        info_block(handler_type &handler, int fd);

    private:
        handler_type *m_handler_ptr{};
        int m_fd;
    };
}


#endif //URING_PROJECT_INFO_BLOCK_H
