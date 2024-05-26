

file(GLOB_RECURSE COROUTINE_HEADERS
        ${PROJECT_SOURCE_DIR}/src/include/coroutine/*.h
        ${PROJECT_SOURCE_DIR}/src/include/coroutine/*.hpp
)

add_library(coroutine1 STATIC awaiter.cpp ${COROUTINE_HEADERS})

target_include_directories(coroutine1 PUBLIC ${PROJECT_SOURCE_DIR}/src/include)
