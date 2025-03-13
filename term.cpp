#include <cstdarg>
#include <cstdio>
#include <conio.h>
#include <cstring>
#include "fifo.h"
#include "term.h"

#define FIFO_SIZE 1024  // 确保是2的幂
static uint8_t fifo_buffer[FIFO_SIZE];
static struct fifo kbd_fifo;

// 显示VT100控制字符
void term_output(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

// 捕捉键盘输入转化为VT100控制字符
void term_capture_input(struct fifo* kbd_fifo) {
    char buffer[32];

    while (true) {
        unsigned char c = _getch();
        if (c == 0xE0 || c == 0) {  // Windows扩展键
            char key = _getch();
            switch (key) {
                case 72:  // 上箭头
                    sprintf(buffer, "\033[A");
                    fifo_write(kbd_fifo, (uint8_t*)buffer, strlen(buffer));
                    break;
                case 80:  // 下箭头
                    sprintf(buffer, "\033[B");
                    fifo_write(kbd_fifo, (uint8_t*)buffer, strlen(buffer));
                    break;
                case 77:  // 右箭头
                    sprintf(buffer, "\033[C");
                    fifo_write(kbd_fifo, (uint8_t*)buffer, strlen(buffer));
                    break;
                case 75:  // 左箭头
                    sprintf(buffer, "\033[D");
                    fifo_write(kbd_fifo, (uint8_t*)buffer, strlen(buffer));
                    break;
                case 83:  // Delete键
                    // 发送DELETE的控制序列作为一个完整的序列
                    fifo_write(kbd_fifo, (uint8_t*)"\033[3~", 4);
                    break;
            }
        } else {
            if (c == '\r') {
                fifo_write(kbd_fifo, (uint8_t*)"\r", 1);
            } else if (c == '\b' || c == 8) {  // Backspace键
                fifo_write(kbd_fifo, (uint8_t*)"\b", 1);
            } else if (c == '\t') {
                fifo_write(kbd_fifo, (uint8_t*)"\t", 1);
            } else if (c < 32) {
                // 其他控制字符
                fifo_write(kbd_fifo, &c, 1);
            } else {
                fifo_write(kbd_fifo, &c, 1);
            }
        }
    }
}

