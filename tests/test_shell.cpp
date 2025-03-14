#include <gtest/gtest.h>
#include "shell.h"
#include <cstring>
#include <thread>
#include <chrono>

class ShellTest : public ::testing::Test {
protected:
    struct fifo input_fifo;
    uint8_t buffer[1024];
    Shell* shell;

    void SetUp() override {
        fifo_init(&input_fifo, buffer, 1024);
        shell = new Shell(&input_fifo);
    }

    void TearDown() override {
        delete shell;
    }
};

TEST_F(ShellTest, BasicInputTest) {
    // 模拟输入普通字符
    const char input[] = "echo test";
    shell->test_handle_input(input, strlen(input));
    EXPECT_EQ(shell->get_command_line(), "echo test");
}

TEST_F(ShellTest, BackspaceTest) {
    // 先输入一些字符
    const char input[] = "test";
    shell->test_handle_input(input, strlen(input));
    
    // 模拟退格键
    char backspace = '\b';
    shell->test_handle_input(&backspace, 1);
    
    EXPECT_EQ(shell->get_command_line(), "tes");
    
    // 多次退格
    shell->test_handle_input(&backspace, 1);
    shell->test_handle_input(&backspace, 1);
    shell->test_handle_input(&backspace, 1);
    
    // 应该清空
    EXPECT_EQ(shell->get_command_line(), "");
    
    // 空字符串上退格不应有效果
    shell->test_handle_input(&backspace, 1);
    EXPECT_EQ(shell->get_command_line(), "");
}

TEST_F(ShellTest, CursorMovementTest) {
    // 输入文本
    const char input[] = "test";
    shell->test_handle_input(input, strlen(input));
    
    // 模拟左箭头键序列
    const char left_arrow[] = "\033[D";
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    
    // 验证光标位置
    EXPECT_EQ(shell->get_cursor_position(), 3);
    
    // 再次左移
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 2);
    
    // 右移
    const char right_arrow[] = "\033[C";
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 3);
    
    // 移动到边界
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 0);
    
    // 超出左边界不应有效果
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 0);
    
    // 移动到右边界
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 4);
    
    // 超出右边界不应有效果
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 4);
}

TEST_F(ShellTest, DeleteKeyTest) {
    // 输入文本
    const char input[] = "abcdef";
    shell->test_handle_input(input, strlen(input));
    
    // 移动光标到中间
    const char left_arrow[] = "\033[D";
    for (int i = 0; i < 3; i++) {
        shell->test_handle_input(left_arrow, strlen(left_arrow));
    }
    EXPECT_EQ(shell->get_cursor_position(), 3);
    
    // 使用DELETE键
    const char delete_key[] = "\033[3~";
    shell->test_handle_input(delete_key, strlen(delete_key));
    
    // 验证删除效果
    EXPECT_EQ(shell->get_command_line(), "abcef");
    EXPECT_EQ(shell->get_cursor_position(), 3);
    
    // 再次删除
    shell->test_handle_input(delete_key, strlen(delete_key));
    EXPECT_EQ(shell->get_command_line(), "abcf");
    
    // 移动到末尾
    const char right_arrow[] = "\033[C";
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    
    // 末尾DELETE不应有效果
    shell->test_handle_input(delete_key, strlen(delete_key));
    EXPECT_EQ(shell->get_command_line(), "abcf");
}

TEST_F(ShellTest, MixedInputTest) {
    // 输入、移动光标、删除的混合测试
    shell->test_handle_input("abc", 3);
    
    // 左移光标
    shell->test_handle_input("\033[D", 3);
    EXPECT_EQ(shell->get_cursor_position(), 2);
    
    // 插入字符
    shell->test_handle_input("X", 1);
    EXPECT_EQ(shell->get_command_line(), "abXc");
    EXPECT_EQ(shell->get_cursor_position(), 3);
    
    // 退格
    char backspace = '\b';
    shell->test_handle_input(&backspace, 1);
    EXPECT_EQ(shell->get_command_line(), "abc");
    
    // 移动到开头
    shell->test_handle_input("\033[D", 3);
    shell->test_handle_input("\033[D", 3);
    shell->test_handle_input("\033[D", 3);
    
    // 插入字符
    shell->test_handle_input("Y", 1);
    EXPECT_EQ(shell->get_command_line(), "Yabc");
}

TEST_F(ShellTest, EnterKeyTest) {
    // 输入命令
    shell->test_handle_input("test command", 12);
    
    // 按回车
    shell->test_handle_input("\r", 1);
    
    // 命令应该被执行并清空
    EXPECT_EQ(shell->get_command_line(), "");
    EXPECT_EQ(shell->get_cursor_position(), 0);
    
    // 空命令回车
    shell->test_handle_input("\n", 1);
    EXPECT_EQ(shell->get_command_line(), "");
}

// TEST_F(ShellTest, IncompleteEscapeSequenceTest) {
//     // 输入不完整的转义序列
//     shell->test_handle_input("\033", 1);
    
//     // 等待超时处理
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
//     // 应该将转义字符作为普通字符处理
//     EXPECT_EQ(shell->get_command_line().length(), 1);
    
//     // 另一个不完整序列
//     shell->test_handle_input("\033[", 2);
    
//     // 等待超时处理
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
//     // 应该将整个序列作为普通字符处理
//     EXPECT_EQ(shell->get_command_line().length(), 3);
// }

// 测试光标位置边界条件
TEST_F(ShellTest, CursorPositionBoundaryTest) {
    // 1. 测试光标位置等于命令行长度的情况
    
    // 输入一些文本
    const char input[] = "test";
    shell->test_handle_input(input, strlen(input));
    EXPECT_EQ(shell->get_command_line(), "test");
    EXPECT_EQ(shell->get_cursor_position(), 4); // 光标应该在末尾
    
    // 尝试右移光标，应该无效果，因为光标已经在末尾
    const char right_arrow[] = "\033[C";
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 4); // 光标位置应该不变
    
    // 2. 测试在行尾追加字符
    shell->test_handle_input("X", 1);
    EXPECT_EQ(shell->get_command_line(), "testX");
    EXPECT_EQ(shell->get_cursor_position(), 5);
    
    // 3. 测试DELETE键在行尾的情况
    const char delete_key[] = "\033[3~";
    shell->test_handle_input(delete_key, strlen(delete_key));
    EXPECT_EQ(shell->get_command_line(), "testX"); // 应该没有变化
    EXPECT_EQ(shell->get_cursor_position(), 5);    // 光标位置应该不变
    
    // 4. 测试刷新行尾
    // 先移动光标到中间
    const char left_arrow[] = "\033[D";
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 3);
    
    // 然后移回行尾
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    shell->test_handle_input(right_arrow, strlen(right_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 5);
    
    // 5. 测试清空命令行后的情况
    // 清空命令行
    while (shell->get_command_line().length() > 0) {
        char backspace = '\b';
        shell->test_handle_input(&backspace, 1);
    }
    
    EXPECT_EQ(shell->get_command_line(), "");
    EXPECT_EQ(shell->get_cursor_position(), 0);
    
    // 尝试左移光标，应该无效果
    shell->test_handle_input(left_arrow, strlen(left_arrow));
    EXPECT_EQ(shell->get_cursor_position(), 0); // 光标位置应该不变
    
    // 尝试删除键，应该无效果
    char backspace_key = '\b';
    shell->test_handle_input(&backspace_key, 1);
    EXPECT_EQ(shell->get_command_line(), "");
    EXPECT_EQ(shell->get_cursor_position(), 0);
    
    // 尝试DELETE键，应该无效果
    shell->test_handle_input(delete_key, strlen(delete_key));
    EXPECT_EQ(shell->get_command_line(), "");
    EXPECT_EQ(shell->get_cursor_position(), 0);
} 