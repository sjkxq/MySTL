#include <gtest/gtest.h>
#include <sjkxq_stl/list.hpp>
#include <string>

// 测试默认构造函数和基本操作
TEST(ListTest, DefaultConstructor) {
    sjkxq_stl::list<int> lst;
    EXPECT_TRUE(lst.empty());
    EXPECT_EQ(lst.size(), 0);
}

// 测试带初始大小的构造函数
TEST(ListTest, SizeConstructor) {
    sjkxq_stl::list<int> lst(5);
    EXPECT_FALSE(lst.empty());
    EXPECT_EQ(lst.size(), 5);
}

// 测试初始化列表构造函数
TEST(ListTest, InitializerListConstructor) {
    sjkxq_stl::list<int> lst{1, 2, 3, 4, 5};
    EXPECT_EQ(lst.size(), 5);
    EXPECT_EQ(lst.front(), 1);
    EXPECT_EQ(lst.back(), 5);
}

// 测试push_back和pop_back
TEST(ListTest, PushBackAndPopBack) {
    sjkxq_stl::list<int> lst;
    
    // 测试push_back
    lst.push_back(1);
    EXPECT_EQ(lst.size(), 1);
    EXPECT_EQ(lst.front(), 1);
    EXPECT_EQ(lst.back(), 1);
    
    lst.push_back(2);
    EXPECT_EQ(lst.size(), 2);
    EXPECT_EQ(lst.front(), 1);
    EXPECT_EQ(lst.back(), 2);
    
    // 测试pop_back
    lst.pop_back();
    EXPECT_EQ(lst.size(), 1);
    EXPECT_EQ(lst.front(), 1);
    EXPECT_EQ(lst.back(), 1);
}

// 测试push_front和pop_front
TEST(ListTest, PushFrontAndPopFront) {
    sjkxq_stl::list<int> lst;
    
    // 测试push_front
    lst.push_front(1);
    EXPECT_EQ(lst.size(), 1);
    EXPECT_EQ(lst.front(), 1);
    EXPECT_EQ(lst.back(), 1);
    
    lst.push_front(2);
    EXPECT_EQ(lst.size(), 2);
    EXPECT_EQ(lst.front(), 2);
    EXPECT_EQ(lst.back(), 1);
    
    // 测试pop_front
    lst.pop_front();
    EXPECT_EQ(lst.size(), 1);
    EXPECT_EQ(lst.front(), 1);
    EXPECT_EQ(lst.back(), 1);
}

// 测试元素访问
TEST(ListTest, ElementAccess) {
    sjkxq_stl::list<int> lst{1, 2, 3, 4, 5};
    
    // 测试front()和back()
    EXPECT_EQ(lst.front(), 1);
    EXPECT_EQ(lst.back(), 5);
}

// 测试迭代器
TEST(ListTest, Iterators) {
    sjkxq_stl::list<int> lst{1, 2, 3, 4, 5};
    
    // 测试正向迭代
    int expected = 1;
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    // 测试反向迭代
    expected = 5;
    for (auto it = lst.rbegin(); it != lst.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

// 测试插入和删除
TEST(ListTest, InsertAndErase) {
    sjkxq_stl::list<int> lst{1, 2, 4, 5};
    
    // 在中间插入元素
    auto it = lst.begin();
    ++it;
    ++it;  // 指向4
    lst.insert(it, 3);
    
    // 验证插入结果
    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;
    for (auto val : lst) {
        EXPECT_EQ(val, expected[i++]);
    }
    EXPECT_EQ(lst.size(), 5);
    
    // 删除元素
    it = lst.begin();
    ++it;  // 指向2
    lst.erase(it);
    
    // 验证删除结果
    int expected2[] = {1, 3, 4, 5};
    i = 0;
    for (auto val : lst) {
        EXPECT_EQ(val, expected2[i++]);
    }
    EXPECT_EQ(lst.size(), 4);
}

// 测试清空
TEST(ListTest, Clear) {
    sjkxq_stl::list<int> lst{1, 2, 3, 4, 5};
    EXPECT_EQ(lst.size(), 5);
    
    lst.clear();
    EXPECT_TRUE(lst.empty());
    EXPECT_EQ(lst.size(), 0);
}

// 测试比较运算符
TEST(ListTest, Comparison) {
    sjkxq_stl::list<int> lst1{1, 2, 3};
    sjkxq_stl::list<int> lst2{1, 2, 3};
    sjkxq_stl::list<int> lst3{1, 2, 4};
    sjkxq_stl::list<int> lst4{1, 2};
    
    EXPECT_EQ(lst1, lst2);
    EXPECT_NE(lst1, lst3);
    EXPECT_NE(lst1, lst4);
    EXPECT_LT(lst1, lst3);
    EXPECT_GT(lst3, lst1);
    EXPECT_LE(lst1, lst2);
    EXPECT_GE(lst1, lst2);
}

// 测试复制和移动
TEST(ListTest, CopyAndMove) {
    sjkxq_stl::list<std::string> lst1{"hello", "world"};
    
    // 测试复制构造
    sjkxq_stl::list<std::string> lst2(lst1);
    EXPECT_EQ(lst1, lst2);
    
    // 测试移动构造
    sjkxq_stl::list<std::string> lst3(std::move(lst2));
    EXPECT_EQ(lst1, lst3);
    EXPECT_TRUE(lst2.empty());  // lst2应该被移动走了
    
    // 测试复制赋值
    sjkxq_stl::list<std::string> lst4;
    lst4 = lst1;
    EXPECT_EQ(lst1, lst4);
    
    // 测试移动赋值
    sjkxq_stl::list<std::string> lst5;
    lst5 = std::move(lst4);
    EXPECT_EQ(lst1, lst5);
    EXPECT_TRUE(lst4.empty());  // lst4应该被移动走了
}

// 测试emplace操作
TEST(ListTest, Emplace) {
    sjkxq_stl::list<std::string> lst;
    
    // 测试emplace_back
    lst.emplace_back("hello");
    EXPECT_EQ(lst.size(), 1);
    EXPECT_EQ(lst.back(), "hello");
    
    // 测试emplace_front
    lst.emplace_front("world");
    EXPECT_EQ(lst.size(), 2);
    EXPECT_EQ(lst.front(), "world");
}