

file(GLOB_RECURSE COROUTINE_HEADERS
        ${PROJECT_SOURCE_DIR}/src/include/coroutine/*.h
        ${PROJECT_SOURCE_DIR}/src/include/coroutine/*.hpp
)

add_library(coroutine INTERFACE ${COROUTINE_HEADERS})

target_include_directories(coroutine INTERFACE ${PROJECT_SOURCE_DIR}/src/include)
