#include <gtest/gtest.h>
#include "fifo.h"
#include <cstring> // For memcmp and memset
#include <cstdint> // For uint8_t, uint32_t

class FifoTest : public ::testing::Test {
protected:
    struct fifo fifo;
    uint8_t buffer[1024];

    void SetUp() override {
        fifo_init(&fifo, buffer, 1024);
    }
};

TEST_F(FifoTest, InitializationTest) {
    EXPECT_EQ(fifo.capacity, 1024);
    EXPECT_EQ(fifo.in, 0);
    EXPECT_EQ(fifo.out, 0);
    
    // 测试非2的幂的容量
    struct fifo invalid_fifo;
    uint8_t invalid_buffer[1000]; // 1000不是2的幂
    EXPECT_FALSE(fifo_init(&invalid_fifo, invalid_buffer, 1000));
    // 初始化应该失败，值应该保持不变
    EXPECT_NE(invalid_fifo.capacity, 1000);
}

TEST_F(FifoTest, WriteAndReadTest) {
    uint8_t write_data[] = "Hello";
    uint8_t read_buffer[10];
    
    // 写入测试
    uint32_t written = fifo_write(&fifo, write_data, 5);
    EXPECT_EQ(written, 5);
    EXPECT_EQ(fifo.in, 5);
    
    // 读取测试
    uint32_t read = fifo_read(&fifo, read_buffer, 5);
    EXPECT_EQ(read, 5);
    EXPECT_EQ(memcmp(write_data, read_buffer, 5), 0);
    EXPECT_EQ(fifo.out, 5);
}

TEST_F(FifoTest, CircularBufferTest) {
    // 填满缓冲区
    uint8_t data[1024];
    memset(data, 'A', 1024);
    uint32_t written = fifo_write(&fifo, data, 1024);
    EXPECT_EQ(written, 1024);
    
    // 读取一半
    uint8_t read_buffer[512];
    uint32_t read = fifo_read(&fifo, read_buffer, 512);
    EXPECT_EQ(read, 512);
    
    // 写入新数据
    memset(data, 'B', 512);
    written = fifo_write(&fifo, data, 512);
    EXPECT_EQ(written, 512);
    
    // 读取剩余的'A'数据
    read = fifo_read(&fifo, read_buffer, 512);
    EXPECT_EQ(read, 512);
    for (int i = 0; i < 512; i++) {
        EXPECT_EQ(read_buffer[i], 'A');
    }
    
    // 读取'B'数据
    read = fifo_read(&fifo, read_buffer, 512);
    EXPECT_EQ(read, 512);
    for (int i = 0; i < 512; i++) {
        EXPECT_EQ(read_buffer[i], 'B');
    }
}

TEST_F(FifoTest, PeekTest) {
    // 写入数据
    uint8_t write_data[] = "Hello World";
    uint32_t written = fifo_write(&fifo, write_data, 11);
    EXPECT_EQ(written, 11);
    
    // 使用peek查看数据
    uint8_t peek_buffer[11];
    uint32_t peeked = fifo_peek(&fifo, peek_buffer, 11);
    EXPECT_EQ(peeked, 11);
    EXPECT_EQ(memcmp(write_data, peek_buffer, 11), 0);
    
    // 确认peek不会移动读指针
    EXPECT_EQ(fifo.out, 0);
    
    // 再次peek，但只查看部分数据
    memset(peek_buffer, 0, 11);
    peeked = fifo_peek(&fifo, peek_buffer, 5);
    EXPECT_EQ(peeked, 5);
    EXPECT_EQ(memcmp(write_data, peek_buffer, 5), 0);
    
    // 确认读指针仍然不变
    EXPECT_EQ(fifo.out, 0);
}

TEST_F(FifoTest, CommitReadTest) {
    // 写入数据
    uint8_t write_data[] = "Hello World";
    uint32_t written = fifo_write(&fifo, write_data, 11);
    EXPECT_EQ(written, 11);
    
    // 使用peek查看数据
    uint8_t peek_buffer[11];
    uint32_t peeked = fifo_peek(&fifo, peek_buffer, 11);
    EXPECT_EQ(peeked, 11);
    
    // 提交部分读取
    uint32_t committed = fifo_commit_read(&fifo, 5);
    EXPECT_EQ(committed, 5);
    EXPECT_EQ(fifo.out, 5);
    
    // 读取剩余数据
    uint8_t read_buffer[11];
    uint32_t read = fifo_read(&fifo, read_buffer, 11);
    EXPECT_EQ(read, 6); // 只剩下6个字节
    EXPECT_EQ(memcmp(write_data + 5, read_buffer, 6), 0);
}

TEST_F(FifoTest, CircularPeekTest) {
    // 填满大部分缓冲区
    uint8_t data[900];
    memset(data, 'A', 900);
    uint32_t written = fifo_write(&fifo, data, 900);
    EXPECT_EQ(written, 900);
    
    // 读取一部分
    uint8_t read_buffer[500];
    uint32_t read = fifo_read(&fifo, read_buffer, 500);
    EXPECT_EQ(read, 500);
    
    // 写入更多数据，使缓冲区环绕
    memset(data, 'B', 600);
    written = fifo_write(&fifo, data, 600);
    EXPECT_EQ(written, 600);
    
    // 使用peek查看跨越环形缓冲区边界的数据
    uint8_t peek_buffer[1000];
    uint32_t peeked = fifo_peek(&fifo, peek_buffer, 1000);
    EXPECT_EQ(peeked, 1000); // 400个'A' + 600个'B'
    
    // 验证数据正确性
    for (int i = 0; i < 400; i++) {
        EXPECT_EQ(peek_buffer[i], 'A');
    }
    for (int i = 400; i < 1000; i++) {
        EXPECT_EQ(peek_buffer[i], 'B');
    }
}

TEST_F(FifoTest, AvailabilityFunctionsTest) {
    // 初始状态
    EXPECT_EQ(fifo_write_available(&fifo), 1024);
    EXPECT_EQ(fifo_read_available(&fifo), 0);
    
    // 写入一些数据
    uint8_t data[300];
    memset(data, 'X', 300);
    fifo_write(&fifo, data, 300);
    
    // 检查可用空间
    EXPECT_EQ(fifo_write_available(&fifo), 724);
    EXPECT_EQ(fifo_read_available(&fifo), 300);
    
    // 读取一些数据
    uint8_t read_buffer[100];
    fifo_read(&fifo, read_buffer, 100);
    
    // 再次检查可用空间
    EXPECT_EQ(fifo_write_available(&fifo), 824);
    EXPECT_EQ(fifo_read_available(&fifo), 200);
    
    // 测试边界函数
    uint32_t write_pos = fifo.in & (fifo.capacity - 1);
    EXPECT_EQ(fifo_write_available_to_end(&fifo), fifo.capacity - write_pos);
    
    uint32_t read_pos = fifo.out & (fifo.capacity - 1);
    EXPECT_EQ(fifo_read_available_to_end(&fifo), fifo.capacity - read_pos);
}

TEST_F(FifoTest, EdgeCasesTest) {
    // 尝试从空FIFO读取
    uint8_t read_buffer[10];
    uint32_t read = fifo_read(&fifo, read_buffer, 10);
    EXPECT_EQ(read, 0);
    
    // 尝试写入超过容量的数据
    uint8_t data[2000]; // 超过FIFO容量
    uint32_t written = fifo_write(&fifo, data, 2000);
    EXPECT_EQ(written, 1024); // 只能写入1024字节
    
    // 填满FIFO后尝试再写入
    fifo_init(&fifo, buffer, 1024); // 重置FIFO
    written = fifo_write(&fifo, data, 1024);
    EXPECT_EQ(written, 1024);
    
    uint8_t more_data[10];
    written = fifo_write(&fifo, more_data, 10);
    EXPECT_EQ(written, 0); // 无法写入更多数据
    
    // 尝试提交超过可用数据的读取
    fifo_init(&fifo, buffer, 1024); // 重置FIFO
    fifo_write(&fifo, data, 100);
    uint32_t committed = fifo_commit_read(&fifo, 200);
    EXPECT_EQ(committed, 100); // 只能提交100字节
}

TEST_F(FifoTest, PeekWraparoundTest) {
    // 设置一个特殊的场景，使读取位置靠近缓冲区末尾
    // 首先填充一些数据
    uint8_t data[1024];
    for (int i = 0; i < 1024; i++) {
        data[i] = i % 256;  // 使用不同的值填充
    }
    
    // 写入一些数据
    uint32_t written = fifo_write(&fifo, data, 1000);
    EXPECT_EQ(written, 1000);
    
    // 读取大部分数据，使读指针接近缓冲区末尾
    uint8_t read_buffer[980];
    uint32_t read = fifo_read(&fifo, read_buffer, 980);
    EXPECT_EQ(read, 980);
    
    // 此时读指针应该在位置980
    EXPECT_EQ(fifo.out, 980);
    
    // 写入新数据，使其环绕到缓冲区开始
    uint8_t new_data[100];
    for (int i = 0; i < 100; i++) {
        new_data[i] = 0xA0 + i;  // 使用不同的模式
    }
    written = fifo_write(&fifo, new_data, 100);
    EXPECT_EQ(written, 100);
    
    // 现在FIFO中有20个旧数据(位置980-999)和100个新数据(其中一部分环绕到了开头)
    // 使用peek读取所有剩余数据，这将触发环绕读取
    uint8_t peek_buffer[120];
    uint32_t peeked = fifo_peek(&fifo, peek_buffer, 120);
    EXPECT_EQ(peeked, 120);
    
    // 验证数据正确性
    // 前20个字节应该是原始数据的最后20个字节
    for (int i = 0; i < 20; i++) {
        EXPECT_EQ(peek_buffer[i], (980 + i) % 256);
    }
    
    // 后100个字节应该是新写入的数据
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(peek_buffer[20 + i], (uint8_t)(0xA0 + i));
    }
    
    // 确认peek不会移动读指针
    EXPECT_EQ(fifo.out, 980);
    
    // 现在使用正常的read读取，应该得到相同的数据
    uint8_t read_buffer2[120];
    read = fifo_read(&fifo, read_buffer2, 120);
    EXPECT_EQ(read, 120);
    
    // 验证数据一致性
    EXPECT_EQ(memcmp(peek_buffer, read_buffer2, 120), 0);
    
    // 读指针应该已经移动
    EXPECT_EQ(fifo.out, 1100);
    
    // 验证实际的物理位置是正确的
    EXPECT_EQ(fifo.out & (fifo.capacity - 1), 76);  // 1100 % 1024 = 76
    
    // 额外测试：确保我们可以继续读写
    // 写入更多数据
    uint8_t more_data[50];
    memset(more_data, 0xCC, 50);
    written = fifo_write(&fifo, more_data, 50);
    EXPECT_EQ(written, 50);
    
    // 读取这些数据
    uint8_t final_buffer[50];
    read = fifo_read(&fifo, final_buffer, 50);
    EXPECT_EQ(read, 50);
    
    // 验证数据
    for (int i = 0; i < 50; i++) {
        EXPECT_EQ(final_buffer[i], 0xCC);
    }
    
    // 最终读指针位置
    EXPECT_EQ(fifo.out, 1150);
    EXPECT_EQ(fifo.out & (fifo.capacity - 1), 126);  // 1150 % 1024 = 126
} 