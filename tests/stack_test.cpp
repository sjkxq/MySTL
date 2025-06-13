#include <gtest/gtest.h>
#include <sjkxq_stl/stack.hpp>
#include <vector>
#include <string>

// 测试默认构造函数和基本操作
TEST(StackTest, DefaultConstructor) {
    sjkxq_stl::stack<int> s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}

// 测试使用不同底层容器的构造函数
TEST(StackTest, ContainerConstructor) {
    std::vector<int> v = {1, 2, 3, 4, 5};
    sjkxq_stl::stack<int, std::vector<int>> s(v);
    
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.size(), 5);
    EXPECT_EQ(s.top(), 5);  // 栈顶是最后一个元素
}

// 测试push和pop
TEST(StackTest, PushAndPop) {
    sjkxq_stl::stack<int> s;
    
    // 测试push
    s.push(1);
    EXPECT_EQ(s.size(), 1);
    EXPECT_EQ(s.top(), 1);
    
    s.push(2);
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(s.top(), 2);
    
    s.push(3);
    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s.top(), 3);
    
    // 测试pop
    s.pop();
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(s.top(), 2);
    
    s.pop();
    EXPECT_EQ(s.size(), 1);
    EXPECT_EQ(s.top(), 1);
    
    s.pop();
    EXPECT_TRUE(s.empty());
}

// 测试移动构造和移动赋值
TEST(StackTest, MoveOperations) {
    sjkxq_stl::stack<std::string> s1;
    s1.push("hello");
    s1.push("world");
    
    // 测试移动构造
    sjkxq_stl::stack<std::string> s2(std::move(s1));
    EXPECT_EQ(s2.size(), 2);
    EXPECT_EQ(s2.top(), "world");
    
    // s1现在处于有效但未指定的状态，我们不对它做任何假设
    
    // 测试移动赋值
    sjkxq_stl::stack<std::string> s3;
    s3 = std::move(s2);
    EXPECT_EQ(s3.size(), 2);
    EXPECT_EQ(s3.top(), "world");
}

// 测试复制构造和复制赋值
TEST(StackTest, CopyOperations) {
    sjkxq_stl::stack<int> s1;
    s1.push(1);
    s1.push(2);
    
    // 测试复制构造
    sjkxq_stl::stack<int> s2(s1);
    EXPECT_EQ(s2.size(), 2);
    EXPECT_EQ(s2.top(), 2);
    
    // 原栈应该保持不变
    EXPECT_EQ(s1.size(), 2);
    EXPECT_EQ(s1.top(), 2);
    
    // 测试复制赋值
    sjkxq_stl::stack<int> s3;
    s3 = s1;
    EXPECT_EQ(s3.size(), 2);
    EXPECT_EQ(s3.top(), 2);
    
    // 原栈应该保持不变
    EXPECT_EQ(s1.size(), 2);
    EXPECT_EQ(s1.top(), 2);
}

// 测试emplace
TEST(StackTest, Emplace) {
    sjkxq_stl::stack<std::string> s;
    
    s.emplace("hello");
    EXPECT_EQ(s.size(), 1);
    EXPECT_EQ(s.top(), "hello");
    
    s.emplace("world");
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(s.top(), "world");
}

// 测试swap
TEST(StackTest, Swap) {
    sjkxq_stl::stack<int> s1;
    s1.push(1);
    s1.push(2);
    
    sjkxq_stl::stack<int> s2;
    s2.push(3);
    s2.push(4);
    s2.push(5);
    
    // 使用成员函数swap
    s1.swap(s2);
    
    EXPECT_EQ(s1.size(), 3);
    EXPECT_EQ(s1.top(), 5);
    
    EXPECT_EQ(s2.size(), 2);
    EXPECT_EQ(s2.top(), 2);
    
    // 使用非成员函数swap
    swap(s1, s2);
    
    EXPECT_EQ(s1.size(), 2);
    EXPECT_EQ(s1.top(), 2);
    
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s2.top(), 5);
}

// 测试比较运算符
TEST(StackTest, Comparison) {
    sjkxq_stl::stack<int> s1;
    s1.push(1);
    s1.push(2);
    s1.push(3);
    
    sjkxq_stl::stack<int> s2;
    s2.push(1);
    s2.push(2);
    s2.push(3);
    
    sjkxq_stl::stack<int> s3;
    s3.push(1);
    s3.push(2);
    s3.push(4);
    
    sjkxq_stl::stack<int> s4;
    s4.push(1);
    s4.push(2);
    
    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 != s2);
    
    EXPECT_TRUE(s1 != s3);
    EXPECT_TRUE(s1 < s3);
    EXPECT_TRUE(s1 <= s3);
    EXPECT_FALSE(s1 > s3);
    EXPECT_FALSE(s1 >= s3);
    
    EXPECT_TRUE(s1 != s4);
    EXPECT_FALSE(s1 < s4);
    EXPECT_FALSE(s1 <= s4);
    EXPECT_TRUE(s1 > s4);
    EXPECT_TRUE(s1 >= s4);
}