#include <cstdarg>
#include <cstdio>
#include <conio.h>
#include <cstring>
#include "fifo.h"
#include "term.h"
#include <map>

#define FIFO_SIZE 1024  // 确保是2的幂
static uint8_t fifo_buffer[FIFO_SIZE];
static struct fifo kbd_fifo;

// 默认按键映射定义
static const std::vector<KeyDef> default_windows_mappings = {
    // 方向键
    {KeyType::EXTENDED, 0x48, "\033[A", "UP",    false},    // 上
    {KeyType::EXTENDED, 0x50, "\033[B", "DOWN",  false},    // 下
    {KeyType::EXTENDED, 0x4D, "\033[C", "RIGHT", false},    // 右
    {KeyType::EXTENDED, 0x4B, "\033[D", "LEFT",  false},    // 左
    
    // 编辑键
    {KeyType::EXTENDED, 0x53, "\033[3~", "DELETE", false},  // Delete
    {KeyType::CONTROL,  0x08, "\b",      "BACKSPACE", false}, // Backspace
    {KeyType::CONTROL,  0x09, "\t",      "TAB", false},     // Tab
    {KeyType::CONTROL,  0x0D, "\r",      "ENTER", false},   // Enter
    
    // Ctrl组合键 (0x01-0x1A 对应 Ctrl+A 到 Ctrl+Z)
    {KeyType::CONTROL,  0x01, "\x01", "CTRL+A", false},     // Ctrl+A (移动到行首)
    {KeyType::CONTROL,  0x02, "\x02", "CTRL+B", false},     // Ctrl+B (向后移动)
    {KeyType::CONTROL,  0x03, "\x03", "CTRL+C", false},     // Ctrl+C (中断)
    {KeyType::CONTROL,  0x04, "\x04", "CTRL+D", false},     // Ctrl+D (删除/EOF)
    {KeyType::CONTROL,  0x05, "\x05", "CTRL+E", false},     // Ctrl+E (移动到行尾)
    {KeyType::CONTROL,  0x06, "\x06", "CTRL+F", false},     // Ctrl+F (向前移动)
    {KeyType::CONTROL,  0x07, "\x07", "CTRL+G", false},     // Ctrl+G (响铃)
    {KeyType::CONTROL,  0x08, "\x08", "CTRL+H", false},     // Ctrl+H (退格)
    {KeyType::CONTROL,  0x09, "\x09", "CTRL+I", false},     // Ctrl+I (Tab)
    {KeyType::CONTROL,  0x0A, "\x0A", "CTRL+J", false},     // Ctrl+J (换行)
    {KeyType::CONTROL,  0x0B, "\x0B", "CTRL+K", false},     // Ctrl+K (删除到行尾)
    {KeyType::CONTROL,  0x0C, "\x0C", "CTRL+L", false},     // Ctrl+L (清屏)
    {KeyType::CONTROL,  0x0D, "\x0D", "CTRL+M", false},     // Ctrl+M (回车)
    {KeyType::CONTROL,  0x0E, "\x0E", "CTRL+N", false},     // Ctrl+N (下一个)
    {KeyType::CONTROL,  0x0F, "\x0F", "CTRL+O", false},     // Ctrl+O
    {KeyType::CONTROL,  0x10, "\x10", "CTRL+P", false},     // Ctrl+P (上一个)
    {KeyType::CONTROL,  0x11, "\x11", "CTRL+Q", false},     // Ctrl+Q
    {KeyType::CONTROL,  0x12, "\x12", "CTRL+R", false},     // Ctrl+R (反向搜索)
    {KeyType::CONTROL,  0x13, "\x13", "CTRL+S", false},     // Ctrl+S (正向搜索)
    {KeyType::CONTROL,  0x14, "\x14", "CTRL+T", false},     // Ctrl+T (交换字符)
    {KeyType::CONTROL,  0x15, "\x15", "CTRL+U", false},     // Ctrl+U (删除到行首)
    {KeyType::CONTROL,  0x16, "\x16", "CTRL+V", false},     // Ctrl+V
    {KeyType::CONTROL,  0x17, "\x17", "CTRL+W", false},     // Ctrl+W (删除前一个单词)
    {KeyType::CONTROL,  0x18, "\x18", "CTRL+X", false},     // Ctrl+X
    {KeyType::CONTROL,  0x19, "\x19", "CTRL+Y", false},     // Ctrl+Y (粘贴)
    {KeyType::CONTROL,  0x1A, "\x1A", "CTRL+Z", false},     // Ctrl+Z (挂起)

    // 其他控制字符
    {KeyType::CONTROL,  0x1B, "\x1B", "ESC", false},        // ESC
    {KeyType::CONTROL,  0x1C, "\x1C", "CTRL+\\", false},    // Ctrl+\ (退出)
    {KeyType::CONTROL,  0x1D, "\x1D", "CTRL+]", false},     // Ctrl+]
    {KeyType::CONTROL,  0x1E, "\x1E", "CTRL+^", false},     // Ctrl+^
    {KeyType::CONTROL,  0x1F, "\x1F", "CTRL+_", false},     // Ctrl+_
};

#ifdef __linux__
static const std::vector<KeyDef> default_linux_mappings = {
    // Linux特定的按键映射
    // ...
};
#endif

void KeyMap::init_default_mappings() {
#ifdef _WIN32
    for (const auto& mapping : default_windows_mappings) {
        add_mapping(mapping);
    }
#elif defined(__linux__)
    for (const auto& mapping : default_linux_mappings) {
        add_mapping(mapping);
    }
#endif
}


// 捕捉键盘输入转化为VT100控制字符
void term_capture_input(struct fifo* kbd_fifo) {
    KeyMap& key_map = KeyMap::instance();

    while (true) {
        // 1. 获取键盘输入
        unsigned char c = _getch();
        std::string vt100_seq;

        // 2. 判断输入类型
        if (c == 0xE0 || c == 0) {  // Windows扩展键
            // 2.1 Windows扩展键处理
            char key = _getch();  // 获取扩展键的实际键值
            vt100_seq = key_map.get_vt100_sequence(key, KeyType::EXTENDED);
        } else {
            // 2.2 普通键处理
            KeyType type = (c < 32) ? KeyType::CONTROL : KeyType::NORMAL;
            vt100_seq = key_map.get_vt100_sequence(c, type);
        }

        // 3. 写入FIFO
        if (!vt100_seq.empty()) {
            fifo_write(kbd_fifo, (uint8_t*)vt100_seq.c_str(), vt100_seq.length());
        }
    }
}

// KeyMap实现
KeyMap& KeyMap::instance() {
    static KeyMap instance;
    return instance;
}

KeyMap::KeyMap() {
    init_default_mappings();
}

std::string KeyMap::get_vt100_sequence(int platform_code, KeyType type) {
    for (const auto& mapping : key_mappings) {
        if (mapping.platform_code == platform_code && mapping.type == type) {
            return mapping.vt100_seq;
        }
    }
    // 如果是普通可打印字符，直接返回对应的字符
    if (type == KeyType::NORMAL && platform_code >= 32) {
        return std::string(1, (char)platform_code);
    }
    return "";
}

void KeyMap::add_mapping(const KeyDef& key_def) {
    // 检查是否已存在相同的映射
    for (auto& mapping : key_mappings) {
        if (mapping.platform_code == key_def.platform_code && 
            mapping.type == key_def.type) {
            mapping = key_def;  // 更新现有映射
            return;
        }
    }
    key_mappings.push_back(key_def);  // 添加新映射
}
