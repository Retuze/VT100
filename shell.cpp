#include "shell.h"
#include <cstdio>
#include <cstring>
#include <chrono>

void Shell::move_cursor(int n) {
    if (n > 0) {
        printf("\033[%dC", n);  // 右移
    } else if (n < 0) {
        printf("\033[%dD", -n); // 左移
    }
}

void Shell::clear_line() {
    printf("\r\033[K");  // 回到行首并清除整行
}

void Shell::refresh_line() {
    clear_line();
    printf("$ %s", command_line.c_str());
    // 将光标移动到正确位置
    move_cursor(cursor_pos - command_line.length());
    fflush(stdout);
}

void Shell::handle_escape_sequence() {
    if (escape_pos < 2) return;
    
    if (escape_buffer[1] == '[') {
        if (escape_pos >= 3) {
            switch (escape_buffer[2]) {
                case 'A':  // 上箭头
                    if (history_pos > 0) {
                        history_pos--;
                        command_line = history[history_pos];
                        cursor_pos = command_line.length();
                        refresh_line();
                    }
                    break;
                case 'B':  // 下箭头
                    if (history_pos < history.size()) {
                        history_pos++;
                        if (history_pos == history.size()) {
                            command_line.clear();
                        } else {
                            command_line = history[history_pos];
                        }
                        cursor_pos = command_line.length();
                        refresh_line();
                    }
                    break;
                case 'C':  // 右箭头
                    if (cursor_pos < command_line.length()) {
                        cursor_pos++;
                        move_cursor(1);
                    }
                    break;
                case 'D':  // 左箭头
                    if (cursor_pos > 0) {
                        cursor_pos--;
                        move_cursor(-1);
                    }
                    break;
                case '3':  // Delete键序列开始
                    if (escape_pos >= 4 && escape_buffer[3] == '~') {
                        if (cursor_pos < command_line.length()) {
                            command_line.erase(cursor_pos, 1);
                            refresh_line();
                        }
                    }
                    break;
            }
        }
    }
}

void Shell::handle_input(const char* seq, size_t len) {
    auto now = std::chrono::steady_clock::now();

    for (size_t i = 0; i < len; i++) {
        char c = seq[i];

        // 检查超时
        if (input_state != NORMAL) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - last_input_time).count();
            if (elapsed > ESCAPE_TIMEOUT_MS) {
                input_state = NORMAL;
                escape_pos = 0;
            }
        }

        last_input_time = now;

        switch (input_state) {
            case NORMAL:
                if (c == '\033') {
                    input_state = ESC_RECEIVED;
                    escape_buffer[0] = c;
                    escape_pos = 1;
                } else if (c == '\r' || c == '\n') {
                    printf("\n");
                    if (!command_line.empty()) {
                        history.push_back(command_line);
                        history_pos = history.size();
                        execute_command(command_line);
                        command_line.clear();
                        cursor_pos = 0;
                    }
                    printf("$ ");
                    fflush(stdout);
                } else if (c == '\b' || c == 8) {
                    if (cursor_pos > 0) {
                        command_line.erase(cursor_pos - 1, 1);
                        cursor_pos--;
                        refresh_line();
                    }
                } else {
                    command_line.insert(cursor_pos, 1, c);
                    cursor_pos++;
                    refresh_line();
                }
                break;

            case ESC_RECEIVED:
                escape_buffer[escape_pos++] = c;
                if (c == '[') {
                    input_state = CSI_RECEIVED;
                } else {
                    input_state = NORMAL;
                    escape_pos = 0;
                }
                break;

            case CSI_RECEIVED:
                escape_buffer[escape_pos++] = c;
                // 检查是否是Delete序列
                if (escape_pos == 3 && c == '3') {
                    // 等待更多字符
                    continue;
                }
                // 处理完整的Delete序列
                if (escape_pos == 4 && escape_buffer[2] == '3' && c == '~') {
                    if (cursor_pos < command_line.length()) {
                        command_line.erase(cursor_pos, 1);
                        refresh_line();
                    }
                    input_state = NORMAL;
                    escape_pos = 0;
                    break;
                }
                // 处理其他序列
                if (c >= 'A' && c <= 'D') {
                    handle_escape_sequence();
                    input_state = NORMAL;
                    escape_pos = 0;
                }
                break;
        }
    }
}

Shell::Shell(struct fifo* fifo) : 
    cursor_pos(0), 
    history_pos(0), 
    input_fifo(fifo),
    input_state(NORMAL),
    escape_pos(0),
    last_input_time(std::chrono::steady_clock::now()) {
    printf("$ ");
    fflush(stdout);
}

void Shell::process_input() {
    uint8_t buffer[256];
    while (true) {
        uint32_t len = fifo_read(input_fifo, buffer, sizeof(buffer));
        if (len > 0) {
            // printf("Shell received %d bytes\n", len);  // 调试输出
            handle_input((const char*)buffer, len);
        }
    }
}

void Shell::execute_command(const std::string& cmd) {
    printf("Executing command: %s\n", cmd.c_str());
}
