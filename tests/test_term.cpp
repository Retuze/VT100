#include <gtest/gtest.h>
#include "term.h"
#include <cstring>
#include <thread>
#include <chrono>

class TermTest : public ::testing::Test {
protected:
    struct fifo kbd_fifo;
    uint8_t buffer[1024];

    void SetUp() override {
        fifo_init(&kbd_fifo, buffer, 1024);
    }
};

TEST_F(TermTest, KeyMapSingletonTest) {
    KeyMap& map1 = KeyMap::instance();
    KeyMap& map2 = KeyMap::instance();
    EXPECT_EQ(&map1, &map2);
}

TEST_F(TermTest, KeyMapDefaultMappingsTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试方向键映射
    std::string up_seq = key_map.get_vt100_sequence(0x48, KeyType::EXTENDED);
    EXPECT_EQ(up_seq, "\033[A");
    
    // 测试控制键映射
    std::string ctrl_c = key_map.get_vt100_sequence(0x03, KeyType::CONTROL);
    EXPECT_EQ(ctrl_c, "\x03");
}

TEST_F(TermTest, KeyMapCustomMappingTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 添加自定义映射
    KeyDef custom_key = {KeyType::NORMAL, 'X', "\033[X]", "CUSTOM", false};
    key_map.add_mapping(custom_key);
    
    // 验证映射
    std::string seq = key_map.get_vt100_sequence('X', KeyType::NORMAL);
    EXPECT_EQ(seq, "\033[X]");
    
    // 更新现有映射
    KeyDef updated_key = {KeyType::NORMAL, 'X', "\033[NEW]", "UPDATED", false};
    key_map.add_mapping(updated_key);
    
    // 验证更新后的映射
    seq = key_map.get_vt100_sequence('X', KeyType::NORMAL);
    EXPECT_EQ(seq, "\033[NEW]");
}

TEST_F(TermTest, KeyMapNormalCharTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试普通可打印字符
    for (int c = 32; c < 127; c++) {
        std::string seq = key_map.get_vt100_sequence(c, KeyType::NORMAL);
        EXPECT_EQ(seq, std::string(1, (char)c));
    }
}

TEST_F(TermTest, KeyMapExtendedKeysTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试所有已定义的扩展键
    std::vector<int> extended_keys = {0x48, 0x50, 0x4D, 0x4B, 0x53};
    std::vector<std::string> expected_seqs = {"\033[A", "\033[B", "\033[C", "\033[D", "\033[3~"};
    
    for (size_t i = 0; i < extended_keys.size(); i++) {
        std::string seq = key_map.get_vt100_sequence(extended_keys[i], KeyType::EXTENDED);
        EXPECT_EQ(seq, expected_seqs[i]);
    }
    
    // 测试未定义的扩展键
    std::string seq = key_map.get_vt100_sequence(0x99, KeyType::EXTENDED);
    EXPECT_EQ(seq, "");
}

TEST_F(TermTest, KeyMapControlKeysTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试所有控制键 (Ctrl+A 到 Ctrl+Z)
    for (int i = 1; i <= 26; i++) {
        std::string seq = key_map.get_vt100_sequence(i, KeyType::CONTROL);
        EXPECT_EQ(seq, std::string(1, (char)i));
    }
    
    // 测试其他控制键
    std::vector<int> control_keys = {0x08, 0x09, 0x0D, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
    for (int key : control_keys) {
        std::string seq = key_map.get_vt100_sequence(key, KeyType::CONTROL);
        EXPECT_FALSE(seq.empty());
    }
}


// 模拟键盘输入的辅助函数
void simulate_key_press(struct fifo* fifo, int key, KeyType type) {
    KeyMap& key_map = KeyMap::instance();
    std::string seq = key_map.get_vt100_sequence(key, type);
    if (!seq.empty()) {
        fifo_write(fifo, (uint8_t*)seq.c_str(), seq.length());
    }
}

// 注意：term_capture_input是一个无限循环函数，很难直接测试
// 我们可以创建一个模拟版本进行测试
TEST_F(TermTest, SimulatedCaptureInputTest) {
    // 模拟按下几个键
    simulate_key_press(&kbd_fifo, 'A', KeyType::NORMAL);
    simulate_key_press(&kbd_fifo, 0x48, KeyType::EXTENDED); // UP
    simulate_key_press(&kbd_fifo, 0x03, KeyType::CONTROL);  // Ctrl+C
    
    // 读取FIFO中的数据
    uint8_t read_buffer[100];
    uint32_t read = fifo_read(&kbd_fifo, read_buffer, 100);
    
    // 验证数据
    EXPECT_EQ(read, 5); // 'A' (1) + UP ("\033[A", 3) + Ctrl+C (1) = 5
    
    // 重置FIFO
    fifo_init(&kbd_fifo, buffer, 1024);
    
    // 测试更多组合
    simulate_key_press(&kbd_fifo, 0x09, KeyType::CONTROL); // Tab
    simulate_key_press(&kbd_fifo, 0x53, KeyType::EXTENDED); // Delete
    
    read = fifo_read(&kbd_fifo, read_buffer, 100);
    EXPECT_GT(read, 0);
}

// 添加测试不同类型的键映射
TEST_F(TermTest, KeyMapDifferentTypesTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试函数键
    KeyDef f1_key = {KeyType::FUNCTION, 0x3B, "\033OP", "F1", false};
    key_map.add_mapping(f1_key);
    std::string seq = key_map.get_vt100_sequence(0x3B, KeyType::FUNCTION);
    EXPECT_EQ(seq, "\033OP");
    
    // 测试修饰键
    KeyDef shift_key = {KeyType::MODIFIER, 0x10, "\033[1;2]", "SHIFT", false};
    key_map.add_mapping(shift_key);
    seq = key_map.get_vt100_sequence(0x10, KeyType::MODIFIER);
    EXPECT_EQ(seq, "\033[1;2]");
}

// 测试键映射的边界情况
TEST_F(TermTest, KeyMapEdgeCasesTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试非ASCII字符
    KeyDef unicode_key = {KeyType::NORMAL, 0x2022, "\xE2\x80\xA2", "BULLET", true};
    key_map.add_mapping(unicode_key);
    std::string seq = key_map.get_vt100_sequence(0x2022, KeyType::NORMAL);
    EXPECT_EQ(seq, "\xE2\x80\xA2");
    
    // 测试空序列
    KeyDef empty_key = {KeyType::NORMAL, 0xFF, "", "EMPTY", false};
    key_map.add_mapping(empty_key);
    seq = key_map.get_vt100_sequence(0xFF, KeyType::NORMAL);
    EXPECT_EQ(seq, "");
}

// 测试多个键映射的组合
TEST_F(TermTest, KeyMapCombinationsTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 添加多个映射
    std::vector<KeyDef> mappings = {
        {KeyType::NORMAL, 'A', "a", "LOWER_A", true},
        {KeyType::NORMAL, 'B', "b", "LOWER_B", true},
        {KeyType::NORMAL, 'C', "c", "LOWER_C", true}
    };
    
    for (const auto& mapping : mappings) {
        key_map.add_mapping(mapping);
    }
    
    // 验证所有映射
    EXPECT_EQ(key_map.get_vt100_sequence('A', KeyType::NORMAL), "a");
    EXPECT_EQ(key_map.get_vt100_sequence('B', KeyType::NORMAL), "b");
    EXPECT_EQ(key_map.get_vt100_sequence('C', KeyType::NORMAL), "c");
}

// 测试 KeyMap 的初始化
TEST_F(TermTest, KeyMapInitializationTest) {
    // 创建一个新的 KeyMap 实例
    KeyMap& key_map = KeyMap::instance();
    
    // 验证默认映射是否已加载
    std::string up_arrow = key_map.get_vt100_sequence(0x48, KeyType::EXTENDED);
    EXPECT_EQ(up_arrow, "\033[A");
    
    std::string down_arrow = key_map.get_vt100_sequence(0x50, KeyType::EXTENDED);
    EXPECT_EQ(down_arrow, "\033[B");
    
    std::string right_arrow = key_map.get_vt100_sequence(0x4D, KeyType::EXTENDED);
    EXPECT_EQ(right_arrow, "\033[C");
    
    std::string left_arrow = key_map.get_vt100_sequence(0x4B, KeyType::EXTENDED);
    EXPECT_EQ(left_arrow, "\033[D");
    
    std::string delete_key = key_map.get_vt100_sequence(0x53, KeyType::EXTENDED);
    EXPECT_EQ(delete_key, "\033[3~");
}


// 模拟 term_capture_input 函数的简化版本进行测试
class TermCaptureTest : public ::testing::Test {
protected:
    struct fifo kbd_fifo;
    uint8_t buffer[1024];
    
    void SetUp() override {
        fifo_init(&kbd_fifo, buffer, 1024);
    }
    
    // 模拟 _getch() 返回的键值
    void simulate_getch_input(std::vector<unsigned char> keys) {
        for (unsigned char key : keys) {
            // 模拟处理扩展键
            if (key == 0xE0 || key == 0) {
                // 假设下一个键是扩展键的实际值
                continue;
            }
            
            KeyType type = (key < 32) ? KeyType::CONTROL : KeyType::NORMAL;
            std::string vt100_seq = KeyMap::instance().get_vt100_sequence(key, type);
            
            if (!vt100_seq.empty()) {
                fifo_write(&kbd_fifo, (uint8_t*)vt100_seq.c_str(), vt100_seq.length());
            }
        }
    }
};

TEST_F(TermCaptureTest, BasicInputCaptureTest) {
    // 模拟普通键输入
    simulate_getch_input({'A', 'B', 'C'});
    
    // 读取FIFO中的数据
    uint8_t read_buffer[10];
    uint32_t read = fifo_read(&kbd_fifo, read_buffer, 10);
    
    // 验证数据
    EXPECT_EQ(read, 3);
    EXPECT_EQ(read_buffer[0], 'A');
    EXPECT_EQ(read_buffer[1], 'B');
    EXPECT_EQ(read_buffer[2], 'C');
}

TEST_F(TermCaptureTest, ControlKeyInputTest) {
    // 模拟控制键输入
    simulate_getch_input({0x03, 0x08, 0x09});  // Ctrl+C, Backspace, Tab
    
    // 读取FIFO中的数据
    uint8_t read_buffer[10];
    uint32_t read = fifo_read(&kbd_fifo, read_buffer, 10);
    
    // 验证数据
    EXPECT_EQ(read, 3);
    EXPECT_EQ(read_buffer[0], 0x03);  // Ctrl+C
    EXPECT_EQ(read_buffer[1], 0x08);  // Backspace
    EXPECT_EQ(read_buffer[2], 0x09);  // Tab
}

TEST_F(TermCaptureTest, MixedInputTest) {
    // 模拟混合输入
    simulate_getch_input({'A', 0x03, 'B', 0x08});
    
    // 读取FIFO中的数据
    uint8_t read_buffer[10];
    uint32_t read = fifo_read(&kbd_fifo, read_buffer, 10);
    
    // 验证数据
    EXPECT_EQ(read, 4);
    EXPECT_EQ(read_buffer[0], 'A');
    EXPECT_EQ(read_buffer[1], 0x03);  // Ctrl+C
    EXPECT_EQ(read_buffer[2], 'B');
    EXPECT_EQ(read_buffer[3], 0x08);  // Backspace
}

// 测试 KeyMap 的 add_mapping 方法
TEST_F(TermTest, AddMappingTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试添加新映射
    KeyDef new_key = {KeyType::NORMAL, 'Z', "\033[Z]", "TEST_Z", false};
    key_map.add_mapping(new_key);
    EXPECT_EQ(key_map.get_vt100_sequence('Z', KeyType::NORMAL), "\033[Z]");
    
    // 测试更新现有映射
    KeyDef updated_key = {KeyType::NORMAL, 'Z', "\033[UPDATED]", "UPDATED_Z", false};
    key_map.add_mapping(updated_key);
    EXPECT_EQ(key_map.get_vt100_sequence('Z', KeyType::NORMAL), "\033[UPDATED]");
    
    // 测试添加不同类型的相同键码
    KeyDef normal_a = {KeyType::NORMAL, 'A', "A", "NORMAL_A", true};
    KeyDef control_a = {KeyType::CONTROL, 'A', "\x01", "CONTROL_A", false};
    key_map.add_mapping(normal_a);
    key_map.add_mapping(control_a);
    
    EXPECT_EQ(key_map.get_vt100_sequence('A', KeyType::NORMAL), "A");
    EXPECT_EQ(key_map.get_vt100_sequence('A', KeyType::CONTROL), "\x01");
}

// 测试默认映射的初始化
TEST_F(TermTest, DefaultMappingsInitTest) {
    KeyMap& key_map = KeyMap::instance();
    
    // 测试方向键
    EXPECT_EQ(key_map.get_vt100_sequence(0x48, KeyType::EXTENDED), "\033[A");  // UP
    EXPECT_EQ(key_map.get_vt100_sequence(0x50, KeyType::EXTENDED), "\033[B");  // DOWN
    EXPECT_EQ(key_map.get_vt100_sequence(0x4D, KeyType::EXTENDED), "\033[C");  // RIGHT
    EXPECT_EQ(key_map.get_vt100_sequence(0x4B, KeyType::EXTENDED), "\033[D");  // LEFT
    
    // 测试编辑键
    EXPECT_EQ(key_map.get_vt100_sequence(0x53, KeyType::EXTENDED), "\033[3~"); // DELETE
    EXPECT_EQ(key_map.get_vt100_sequence(0x08, KeyType::CONTROL), "\b");       // BACKSPACE
    EXPECT_EQ(key_map.get_vt100_sequence(0x09, KeyType::CONTROL), "\t");       // TAB
    EXPECT_EQ(key_map.get_vt100_sequence(0x0D, KeyType::CONTROL), "\r");       // ENTER
    
    // 测试Ctrl组合键
    for (int i = 1; i <= 26; i++) {
        char ctrl_key = i;
        std::string expected(1, ctrl_key);
        EXPECT_EQ(key_map.get_vt100_sequence(ctrl_key, KeyType::CONTROL), expected);
    }
    
    // 测试其他控制字符
    EXPECT_EQ(key_map.get_vt100_sequence(0x1B, KeyType::CONTROL), "\x1B");     // ESC
    EXPECT_EQ(key_map.get_vt100_sequence(0x1C, KeyType::CONTROL), "\x1C");     // CTRL+\
    EXPECT_EQ(key_map.get_vt100_sequence(0x1D, KeyType::CONTROL), "\x1D");     // CTRL+]
    EXPECT_EQ(key_map.get_vt100_sequence(0x1E, KeyType::CONTROL), "\x1E");     // CTRL+^
    EXPECT_EQ(key_map.get_vt100_sequence(0x1F, KeyType::CONTROL), "\x1F");     // CTRL+_
}

// 测试 KeyMap 的构造函数
TEST_F(TermTest, KeyMapConstructorTest) {
    // 由于 KeyMap 是单例，我们不能直接测试构造函数
    // 但我们可以通过检查默认映射是否已加载来间接测试
    KeyMap& key_map = KeyMap::instance();
    
    // 检查一些默认映射是否存在
    EXPECT_FALSE(key_map.get_vt100_sequence(0x48, KeyType::EXTENDED).empty());
    EXPECT_FALSE(key_map.get_vt100_sequence(0x50, KeyType::EXTENDED).empty());
    EXPECT_FALSE(key_map.get_vt100_sequence(0x4D, KeyType::EXTENDED).empty());
    EXPECT_FALSE(key_map.get_vt100_sequence(0x4B, KeyType::EXTENDED).empty());
    EXPECT_FALSE(key_map.get_vt100_sequence(0x53, KeyType::EXTENDED).empty());
}

// 测试扩展键和普通键的处理
TEST_F(TermTest, ExtendedAndNormalKeyProcessingTest) {
    // 模拟 term_capture_input 函数中的扩展键处理逻辑
    
    // 模拟扩展键序列: 0xE0 followed by 0x48 (UP arrow)
    KeyMap& key_map = KeyMap::instance();
    std::string vt100_seq = key_map.get_vt100_sequence(0x48, KeyType::EXTENDED);
    EXPECT_EQ(vt100_seq, "\033[A");
    
    // 模拟普通键: 'A'
    vt100_seq = key_map.get_vt100_sequence('A', KeyType::NORMAL);
    EXPECT_EQ(vt100_seq, "A");
    
    // 模拟控制键: Ctrl+C
    vt100_seq = key_map.get_vt100_sequence(0x03, KeyType::CONTROL);
    EXPECT_EQ(vt100_seq, "\x03");
    
    // 模拟未定义的扩展键
    vt100_seq = key_map.get_vt100_sequence(0xFF, KeyType::EXTENDED);
    EXPECT_EQ(vt100_seq, "");
    
    // 模拟未定义的控制键
    vt100_seq = key_map.get_vt100_sequence(0x7F, KeyType::CONTROL);
    EXPECT_EQ(vt100_seq, "");
}



// 测试 FIFO 写入失败的情况
TEST_F(TermTest, FifoWriteFailureTest) {
    // 创建一个很小的 FIFO，使其容易填满
    uint8_t small_buffer[4];
    struct fifo small_fifo;
    fifo_init(&small_fifo, small_buffer, 4);
    
    // 填满 FIFO
    uint8_t data[4] = {1, 2, 3, 4};
    uint32_t written = fifo_write(&small_fifo, data, 4);
    EXPECT_EQ(written, 4);
    
    // 尝试再写入，应该失败
    KeyMap& key_map = KeyMap::instance();
    std::string seq = key_map.get_vt100_sequence('A', KeyType::NORMAL);
    written = fifo_write(&small_fifo, (uint8_t*)seq.c_str(), seq.length());
    EXPECT_EQ(written, 0);  // 写入应该失败
}

// // 测试 term_capture_input 函数的分支覆盖
// TEST_F(TermCaptureTest, CaptureInputBranchCoverageTest) {
//     // 模拟扩展键输入 (0xE0 followed by key)
//     std::vector<unsigned char> extended_keys = {0xE0, 0x48};  // UP arrow
//     simulate_getch_input(extended_keys);
    
//     // 读取FIFO中的数据
//     uint8_t read_buffer[10];
//     uint32_t read = fifo_read(&kbd_fifo, read_buffer, 10);
    
//     // 验证数据 (应该是 ESC [ A)
//     EXPECT_EQ(read, 3);
//     EXPECT_EQ(read_buffer[0], '\033');
//     EXPECT_EQ(read_buffer[1], '[');
//     EXPECT_EQ(read_buffer[2], 'A');
    
//     // 模拟另一种扩展键输入 (0 followed by key)
//     std::vector<unsigned char> extended_keys2 = {0, 0x50};  // DOWN arrow
//     simulate_getch_input(extended_keys2);
    
//     // 读取FIFO中的数据
//     read = fifo_read(&kbd_fifo, read_buffer, 10);
    
//     // 验证数据 (应该是 ESC [ B)
//     EXPECT_EQ(read, 3);
//     EXPECT_EQ(read_buffer[0], '\033');
//     EXPECT_EQ(read_buffer[1], '[');
//     EXPECT_EQ(read_buffer[2], 'B');
    
//     // 测试未定义的扩展键
//     std::vector<unsigned char> undefined_ext_key = {0xE0, 0xFF};
//     simulate_getch_input(undefined_ext_key);
    
//     // 读取FIFO中的数据 - 应该没有数据写入
//     read = fifo_read(&kbd_fifo, read_buffer, 10);
//     EXPECT_EQ(read, 0);
    
//     // 测试未定义的控制键
//     std::vector<unsigned char> undefined_ctrl_key = {0x7F};
//     simulate_getch_input(undefined_ctrl_key);
    
//     // 读取FIFO中的数据 - 应该没有数据写入
//     read = fifo_read(&kbd_fifo, read_buffer, 10);
//     EXPECT_EQ(read, 0);
// }