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

private:
    // 状态机状态定义
    enum InputState {
        NORMAL,         // 普通输入状态
        ESC_RECEIVED,   // 收到ESC
        CSI_RECEIVED    // 收到 CSI (Control Sequence Introducer)
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
    void handle_input(const char* seq, size_t len);
    void handle_escape_sequence();
    void execute_command(const std::string& cmd);
};

#endif

