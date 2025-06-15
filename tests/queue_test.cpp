#include <gtest/gtest.h>
#include <sjkxq_stl/queue.hpp>
#include <string>
#include <vector>

// 测试默认构造函数和基本操作
TEST(QueueTest, DefaultConstructor)
{
  sjkxq_stl::queue<int> q;
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0);
}

// 测试使用不同底层容器的构造函数
TEST(QueueTest, ContainerConstructor)
{
  std::deque<int>                        d = {1, 2, 3, 4, 5};
  sjkxq_stl::queue<int, std::deque<int>> q(d);

  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.size(), 5);
  EXPECT_EQ(q.front(), 1);  // 队列前端是第一个元素
  EXPECT_EQ(q.back(), 5);   // 队列后端是最后一个元素
}

// 测试push和pop
TEST(QueueTest, PushAndPop)
{
  sjkxq_stl::queue<int> q;

  // 测试push
  q.push(1);
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 1);

  q.push(2);
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 1);  // 前端仍然是第一个元素
  EXPECT_EQ(q.back(), 2);   // 后端是新添加的元素

  q.push(3);
  EXPECT_EQ(q.size(), 3);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);

  // 测试pop
  q.pop();
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 2);  // 前端元素被移除
  EXPECT_EQ(q.back(), 3);

  q.pop();
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 3);
  EXPECT_EQ(q.back(), 3);

  q.pop();
  EXPECT_TRUE(q.empty());
}

// 测试移动构造和移动赋值
TEST(QueueTest, MoveOperations)
{
  sjkxq_stl::queue<std::string> q1;
  q1.push("hello");
  q1.push("world");

  // 测试移动构造
  sjkxq_stl::queue<std::string> q2(std::move(q1));
  EXPECT_EQ(q2.size(), 2);
  EXPECT_EQ(q2.front(), "hello");
  EXPECT_EQ(q2.back(), "world");

  // q1现在处于有效但未指定的状态，我们不对它做任何假设

  // 测试移动赋值
  sjkxq_stl::queue<std::string> q3;
  q3 = std::move(q2);
  EXPECT_EQ(q3.size(), 2);
  EXPECT_EQ(q3.front(), "hello");
  EXPECT_EQ(q3.back(), "world");
}

// 测试复制构造和复制赋值
TEST(QueueTest, CopyOperations)
{
  sjkxq_stl::queue<int> q1;
  q1.push(1);
  q1.push(2);

  // 测试复制构造
  sjkxq_stl::queue<int> q2(q1);
  EXPECT_EQ(q2.size(), 2);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 2);

  // 原队列应该保持不变
  EXPECT_EQ(q1.size(), 2);
  EXPECT_EQ(q1.front(), 1);
  EXPECT_EQ(q1.back(), 2);

  // 测试复制赋值
  sjkxq_stl::queue<int> q3;
  q3 = q1;
  EXPECT_EQ(q3.size(), 2);
  EXPECT_EQ(q3.front(), 1);
  EXPECT_EQ(q3.back(), 2);

  // 原队列应该保持不变
  EXPECT_EQ(q1.size(), 2);
  EXPECT_EQ(q1.front(), 1);
  EXPECT_EQ(q1.back(), 2);
}

// 测试emplace
TEST(QueueTest, Emplace)
{
  sjkxq_stl::queue<std::string> q;

  q.emplace("hello");
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), "hello");
  EXPECT_EQ(q.back(), "hello");

  q.emplace("world");
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), "hello");
  EXPECT_EQ(q.back(), "world");
}

// 测试swap
TEST(QueueTest, Swap)
{
  sjkxq_stl::queue<int> q1;
  q1.push(1);
  q1.push(2);

  sjkxq_stl::queue<int> q2;
  q2.push(3);
  q2.push(4);
  q2.push(5);

  // 使用成员函数swap
  q1.swap(q2);

  EXPECT_EQ(q1.size(), 3);
  EXPECT_EQ(q1.front(), 3);
  EXPECT_EQ(q1.back(), 5);

  EXPECT_EQ(q2.size(), 2);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 2);

  // 使用非成员函数swap
  swap(q1, q2);

  EXPECT_EQ(q1.size(), 2);
  EXPECT_EQ(q1.front(), 1);
  EXPECT_EQ(q1.back(), 2);

  EXPECT_EQ(q2.size(), 3);
  EXPECT_EQ(q2.front(), 3);
  EXPECT_EQ(q2.back(), 5);
}

// 测试比较运算符
TEST(QueueTest, Comparison)
{
  sjkxq_stl::queue<int> q1;
  q1.push(1);
  q1.push(2);
  q1.push(3);

  sjkxq_stl::queue<int> q2;
  q2.push(1);
  q2.push(2);
  q2.push(3);

  sjkxq_stl::queue<int> q3;
  q3.push(1);
  q3.push(2);
  q3.push(4);

  sjkxq_stl::queue<int> q4;
  q4.push(1);
  q4.push(2);

  EXPECT_TRUE(q1 == q2);
  EXPECT_FALSE(q1 != q2);

  EXPECT_TRUE(q1 != q3);
  EXPECT_TRUE(q1 < q3);
  EXPECT_TRUE(q1 <= q3);
  EXPECT_FALSE(q1 > q3);
  EXPECT_FALSE(q1 >= q3);

  EXPECT_TRUE(q1 != q4);
  EXPECT_FALSE(q1 < q4);
  EXPECT_FALSE(q1 <= q4);
  EXPECT_TRUE(q1 > q4);
  EXPECT_TRUE(q1 >= q4);
}