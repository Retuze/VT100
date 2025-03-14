set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fprofile-arcs -ftest-coverage")

# 添加测试可执行文件
add_executable(test_fifo test_fifo.cpp)
add_executable(test_term test_term.cpp)
add_executable(test_shell test_shell.cpp)

# 添加测试定义
target_compile_definitions(test_fifo PRIVATE TESTING)
target_compile_definitions(test_term PRIVATE TESTING)
target_compile_definitions(test_shell PRIVATE TESTING)

# 链接测试库
target_link_libraries(test_fifo
    fifo
    gtest
    gtest_main
    pthread
    gcov
)

target_link_libraries(test_term
    term
    fifo
    gtest
    gtest_main
    pthread
    gcov
)

target_link_libraries(test_shell
    shell
    fifo
    gtest
    gtest_main
    pthread
    gcov
)

# 添加测试
include(GoogleTest)
gtest_discover_tests(test_fifo)
gtest_discover_tests(test_term)
gtest_discover_tests(test_shell)
