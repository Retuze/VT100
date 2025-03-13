#include <thread>
#include "term.h"
#include "shell.h"
#include "fifo.h"

// 共享的FIFO缓冲区
#define FIFO_SIZE 1024
static uint8_t kbd_buffer[FIFO_SIZE];
struct fifo kbd_fifo;  // 全局FIFO

void term_thread_func() {
    term_capture_input(&kbd_fifo);  // 传入FIFO指针
}

int main() {
    // 初始化FIFO
    fifo_init(&kbd_fifo, kbd_buffer, FIFO_SIZE);

    // 创建shell实例
    Shell shell(&kbd_fifo);

    // 创建两个线程
    std::thread term_thread(term_thread_func);  // 终端输入线程
    std::thread shell_thread(&Shell::process_input, &shell);  // shell处理线程

    // 等待线程结束
    term_thread.join();
    shell_thread.join();

    return 0;
}

