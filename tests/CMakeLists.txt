# 下载并构建 Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# 启用测试
enable_testing()

# 添加测试可执行文件
add_executable(vector_test vector_test.cpp)
add_executable(list_test list_test.cpp)
add_executable(stack_test stack_test.cpp)
add_executable(queue_test queue_test.cpp)
add_executable(map_test map_test.cpp)
add_executable(set_test set_test.cpp)
add_executable(unordered_map_test unordered_map_test.cpp)
add_executable(unordered_set_test unordered_set_test.cpp)

# 链接Google Test和我们的库
target_link_libraries(vector_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

target_link_libraries(list_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

target_link_libraries(stack_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

target_link_libraries(queue_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

target_link_libraries(map_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

target_link_libraries(set_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

target_link_libraries(unordered_map_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

target_link_libraries(unordered_set_test
    PRIVATE
    gtest
    gtest_main
    sjkxq_stl
)

# 添加到CTest
add_test(NAME vector_test COMMAND vector_test)
add_test(NAME list_test COMMAND list_test)
add_test(NAME stack_test COMMAND stack_test)
add_test(NAME queue_test COMMAND queue_test)
add_test(NAME map_test COMMAND map_test)
add_test(NAME set_test COMMAND set_test)
add_test(NAME unordered_map_test COMMAND unordered_map_test)
add_test(NAME unordered_set_test COMMAND unordered_set_test)