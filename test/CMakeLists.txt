enable_testing()

include(CTest)

find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_CASE_SOURCE_FILE_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach (FILE_NAME IN ITEMS ${TEST_CASE_SOURCE_FILE_LIST})
    cmake_path(GET FILE_NAME STEM TARGET_NAME)
    list(APPEND TARGET_NAME_LIST ${TARGET_NAME})
endforeach ()

foreach (FILE_NAME TARGET_NAME IN ZIP_LISTS TEST_CASE_SOURCE_FILE_LIST;TARGET_NAME_LIST)
    add_executable(${TARGET_NAME} ${FILE_NAME})
    target_link_libraries(${TARGET_NAME} ${GTEST_BOTH_LIBRARIES} coroutine1 uring_loop)
    target_include_directories(${TARGET_NAME} PRIVATE ${GTEST_INCLUDE_DIRS})
    gtest_discover_tests(${TARGET_NAME})
endforeach ()

