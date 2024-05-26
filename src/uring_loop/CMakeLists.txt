

file(GLOB_RECURSE HEADERS
        ${PROJECT_SOURCE_DIR}/src/include/uring_loop/*.h
        ${PROJECT_SOURCE_DIR}/src/include/uring_loop/*.hpp
)

add_library(uring_loop STATIC uring_loop.cpp ${HEADERS})

target_include_directories(uring_loop PUBLIC ${PROJECT_SOURCE_DIR}/src/include)