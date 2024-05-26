find_library(LIB_URING uring)

file(GLOB_RECURSE HEADERS
        ${PROJECT_SOURCE_DIR}/src/include/uring_loop/*.h
        ${PROJECT_SOURCE_DIR}/src/include/uring_loop/*.hpp
)

add_library(uring_loop STATIC uring_loop.cpp ${HEADERS})

target_link_libraries(uring_loop PUBLIC coroutine1  ${LIB_URING})

target_include_directories(uring_loop PUBLIC ${PROJECT_SOURCE_DIR}/src/include)