//
// Created by 12992 on 24-5-28.
//

#include "uring_loop/info_block.h"


namespace uring_project::uring
{
    uint64_t info_block::create_info_block(info_block::handler_type &handler, int fd)
    {
        return reinterpret_cast<uint64_t>(new info_block(handler, fd));
    }

    info_block info_block::parse(uint64_t v)
    {
        auto info_block_ptr = reinterpret_cast<info_block *>(v);
        auto obj = *info_block_ptr;
        delete info_block_ptr;
        return obj;
    }

    info_block::handler_type *info_block::get_handler_ptr() const
    {
        return this->m_handler_ptr;
    }

    int info_block::get_target_fd() const
    {
        return this->m_fd;
    }

    info_block::info_block(info_block::handler_type &handler, int fd)
            : m_handler_ptr(&handler), m_fd(fd)
    {

    }
} // uring
// uring_project