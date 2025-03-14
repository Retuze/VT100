#ifndef _TERM_H_
#define _TERM_H_

#include <string>
#include <vector>
#include "fifo.h"

// 按键类型枚举
enum class KeyType {
    NORMAL,         // 普通字符
    CONTROL,        // 控制字符
    FUNCTION,       // 功能键
    EXTENDED,       // 扩展键
    MODIFIER        // 修饰键
};

// 按键定义结构
struct KeyDef {
    KeyType type;           // 按键类型
    int platform_code;      // 平台特定的键码
    std::string vt100_seq;  // 对应的VT100序列
    std::string name;       // 按键名称（用于调试和配置）
    bool is_printable;      // 是否可打印字符
};

// 按键映射表
class KeyMap {
public:
    static KeyMap& instance();  // 单例模式
    
    // 根据平台键码获取对应的VT100序列
    std::string get_vt100_sequence(int platform_code, KeyType type = KeyType::NORMAL);
    
    // 添加或更新按键映射
    void add_mapping(const KeyDef& key_def);

private:
    KeyMap();  // 私有构造函数
    std::vector<KeyDef> key_mappings;
    void init_default_mappings();
};

void term_capture_input(struct fifo* kbd_fifo);

#endif
