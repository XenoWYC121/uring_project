

add_executable(async_reader main.cpp)

target_link_libraries(async_reader PUBLIC coroutine1 uring_loop)