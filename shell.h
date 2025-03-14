#ifndef _SHELL_H_
#define _SHELL_H_

#include <string>
#include <vector>
#include <chrono>
#include "fifo.h"

class Shell {
public:
    Shell(struct fifo* fifo);
    void process_input();

    // 测试用公共方法
    #ifdef TESTING
    void test_handle_input(const char* seq, size_t len) {
        handle_input(seq, len);
    }
    std::string get_command_line() const {
        return command_line;
    }
    size_t get_cursor_position() const {
        return cursor_pos;
    }
    #endif

private:
    // 状态机状态定义
    enum InputState {
        NORMAL,         // 普通输入状态
        ESC_RECEIVED,   // 收到ESC
        CSI_RECEIVED,   // 收到 CSI (Control Sequence Introducer)
        CSI_PARAMETER   // 参数处理状态
    };

    // CSI序列解析结构
    struct CSISequence {
        std::vector<int> parameters;  // 参数列表
        char final;                   // 最终字符
    };

    // 成员变量
    std::string command_line;  // 当前命令行
    size_t cursor_pos;         // 光标位置
    std::vector<std::string> history;  // 命令历史
    size_t history_pos;        // 历史命令位置
    struct fifo* input_fifo;   // 输入FIFO
    
    InputState input_state;    // 当前输入状态
    char escape_buffer[32];    // 存储转义序列
    size_t escape_pos;         // 转义序列当前位置
    std::chrono::steady_clock::time_point last_input_time;  // 最后输入时间

    static const int ESCAPE_TIMEOUT_MS = 50;  // 转义序列超时时间（毫秒）

    // 私有成员函数
    void move_cursor(int n);
    void clear_line();
    void refresh_line();
    void append_char(char c);
    void refresh_from_cursor();
    void handle_input(const char* seq, size_t len);
    void execute_command(const std::string& cmd);

    // CSI序列处理
    void parse_csi_sequence();
    void handle_csi_sequence(const CSISequence& seq);
    void handle_cursor_movement(char direction, int count);
    void handle_delete_key();
    void handle_history_navigation(int direction);
    bool check_sequence_timeout();
    void handle_incomplete_sequence();
    void reset_sequence_state();
};

#endif

