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

void Shell::append_char(char c) {
    putchar(c);
    fflush(stdout);
}

void Shell::refresh_from_cursor() {
    // 从光标位置刷新到行尾
    printf("\033[K");  // 先清除从光标到行尾的内容
    if (cursor_pos < command_line.length()) {
        // 打印从光标位置到行尾的内容
        const char* remaining = command_line.c_str() + cursor_pos;
        printf("%s", remaining);
        // 将光标移回正确位置，只有当光标不在字符串末尾时才需要移动
        size_t remaining_len = strlen(remaining);
        if (remaining_len > 0) {
            move_cursor(-remaining_len);
        }
    }
    fflush(stdout);
}

void Shell::refresh_line() {
    clear_line();
    printf("$ %s", command_line.c_str());
    move_cursor(cursor_pos - command_line.length());
    fflush(stdout);
}

void Shell::handle_cursor_movement(char direction, int count) {
    switch (direction) {
        case 'A':  // UP
            handle_history_navigation(-1);
            break;
        case 'B':  // DOWN
            handle_history_navigation(1);
            break;
        case 'C':  // RIGHT
            if (cursor_pos < command_line.length()) {
                cursor_pos++;
                move_cursor(1);
            }
            break;
        case 'D':  // LEFT
            if (cursor_pos > 0) {
                cursor_pos--;
                move_cursor(-1);
            }
            break;
    }
}

void Shell::handle_history_navigation(int direction) {
    if (direction < 0 && history_pos > 0) {
        history_pos--;
        command_line = history[history_pos];
        cursor_pos = command_line.length();
        refresh_line();
    } else if (direction > 0 && history_pos < history.size()) {
        history_pos++;
        if (history_pos == history.size()) {
            command_line.clear();
        } else {
            command_line = history[history_pos];
        }
        cursor_pos = command_line.length();
        refresh_line();
    }
}

void Shell::handle_delete_key() {
    if (cursor_pos < command_line.length()) {
        command_line.erase(cursor_pos, 1);
        refresh_line();
    }
}

void Shell::parse_csi_sequence() {
    CSISequence seq;
    std::string current_param;
    
    for (size_t i = 2; i < escape_pos; i++) {
        char c = escape_buffer[i];
        if (c >= '0' && c <= '9') {
            current_param += c;
        } else if (c == ';') {
            if (!current_param.empty()) {
                seq.parameters.push_back(std::stoi(current_param));
                current_param.clear();
            } else {
                seq.parameters.push_back(0);
            }
        } else {  // 最终字符
            if (!current_param.empty()) {
                seq.parameters.push_back(std::stoi(current_param));
            }
            seq.final = c;
            break;
        }
    }
    handle_csi_sequence(seq);
}

void Shell::handle_csi_sequence(const CSISequence& seq) {
    int count = seq.parameters.empty() ? 1 : seq.parameters[0];
    
    switch (seq.final) {
        case 'A': case 'B': case 'C': case 'D':
            handle_cursor_movement(seq.final, count);
            break;
        case '~':
            if (!seq.parameters.empty() && seq.parameters[0] == 3) {
                handle_delete_key();
            }
            break;
    }
}

void Shell::handle_input(const char* seq, size_t len) {
    auto now = std::chrono::steady_clock::now();

    for (size_t i = 0; i < len; i++) {
        char c = seq[i];

        if (check_sequence_timeout()) {
            i--;
            continue;
        }

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
                        // 回退光标并清除当前位置到行尾
                        move_cursor(-1);
                        printf("\033[K");  // 清除从光标到行尾的内容
                        // 如果不是在行尾删除，需要重新显示后面的内容
                        if (cursor_pos < command_line.length()) {
                            printf("%s", command_line.c_str() + cursor_pos);
                            // 将光标移回正确位置
                            move_cursor(-(command_line.length() - cursor_pos));
                        }
                        fflush(stdout);
                    }
                } else if (c >= 32) {  // 可打印字符
                    command_line.insert(cursor_pos, 1, c);
                    cursor_pos++;  // 先增加光标位置
                    if (cursor_pos == command_line.length()) {
                        // 如果在行尾追加，直接输出字符
                        append_char(c);
                    } else {
                        // 如果在行中插入，刷新从当前位置到行尾的内容
                        putchar(c);
                        refresh_from_cursor();  // 刷新后续内容
                    }
                }
                break;

            case ESC_RECEIVED:
                escape_buffer[escape_pos++] = c;
                if (c == '[') {
                    input_state = CSI_RECEIVED;
                } else {
                    reset_sequence_state();
                }
                break;

            case CSI_RECEIVED:
            case CSI_PARAMETER:
                escape_buffer[escape_pos++] = c;
                if ((c >= '0' && c <= '9') || c == ';') {
                    input_state = CSI_PARAMETER;
                } else {
                    parse_csi_sequence();
                    reset_sequence_state();
                }
                break;
        }
        last_input_time = now;
    }
}

bool Shell::check_sequence_timeout() {
    if (input_state == NORMAL) return false;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_input_time).count();
    
    if (elapsed > ESCAPE_TIMEOUT_MS) {
        handle_incomplete_sequence();
        reset_sequence_state();
        return true;
    }
    return false;
}

void Shell::handle_incomplete_sequence() {
    for (size_t i = 0; i < escape_pos; i++) {
        command_line.insert(cursor_pos, 1, escape_buffer[i]);
        cursor_pos++;
    }
    refresh_line();
}

void Shell::reset_sequence_state() {
    input_state = NORMAL;
    escape_pos = 0;
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
            handle_input((const char*)buffer, len);
        }
    }
}

void Shell::execute_command(const std::string& cmd) {
    // printf("Executing command: %s\n", cmd.c_str());
    system(cmd.c_str());
}
