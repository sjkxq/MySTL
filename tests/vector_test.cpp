#include <gtest/gtest.h>
#include <sjkxq_stl/vector.hpp>
#include <string>

// 测试默认构造函数和基本操作
TEST(VectorTest, DefaultConstructor)
{
  sjkxq_stl::vector<int> vec;
  EXPECT_TRUE(vec.empty());
  EXPECT_EQ(vec.size(), 0);
  EXPECT_EQ(vec.capacity(), 0);
}

// 测试带初始大小的构造函数
TEST(VectorTest, SizeConstructor)
{
  sjkxq_stl::vector<int> vec(5);
  EXPECT_FALSE(vec.empty());
  EXPECT_EQ(vec.size(), 5);
  EXPECT_GE(vec.capacity(), 5);
}

// 测试初始化列表构造函数
TEST(VectorTest, InitializerListConstructor)
{
  sjkxq_stl::vector<int> vec{1, 2, 3, 4, 5};
  EXPECT_EQ(vec.size(), 5);
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[4], 5);
}

// 测试push_back和pop_back
TEST(VectorTest, PushAndPop)
{
  sjkxq_stl::vector<int> vec;

  // 测试push_back
  vec.push_back(1);
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec[0], 1);

  vec.push_back(2);
  EXPECT_EQ(vec.size(), 2);
  EXPECT_EQ(vec[1], 2);

  // 测试pop_back
  vec.pop_back();
  EXPECT_EQ(vec.size(), 1);
  EXPECT_EQ(vec[0], 1);
}

// 测试元素访问
TEST(VectorTest, ElementAccess)
{
  sjkxq_stl::vector<int> vec{1, 2, 3, 4, 5};

  // 测试operator[]
  EXPECT_EQ(vec[0], 1);
  EXPECT_EQ(vec[4], 5);

  // 测试at()
  EXPECT_EQ(vec.at(0), 1);
  EXPECT_EQ(vec.at(4), 5);
  EXPECT_THROW(vec.at(5), sjkxq_stl::out_of_range);

  // 测试front()和back()
  EXPECT_EQ(vec.front(), 1);
  EXPECT_EQ(vec.back(), 5);
}

// 测试容量管理
TEST(VectorTest, Capacity)
{
  sjkxq_stl::vector<int> vec;

  // 测试reserve
  vec.reserve(10);
  EXPECT_GE(vec.capacity(), 10);
  EXPECT_EQ(vec.size(), 0);

  // 添加元素测试自动扩容
  for (int i = 0; i < 15; ++i) {
    vec.push_back(i);
  }
  EXPECT_GE(vec.capacity(), 15);
  EXPECT_EQ(vec.size(), 15);

  // 测试shrink_to_fit
  vec.shrink_to_fit();
  EXPECT_EQ(vec.capacity(), vec.size());
}

// 测试迭代器
TEST(VectorTest, Iterators)
{
  sjkxq_stl::vector<int> vec{1, 2, 3, 4, 5};

  // 测试正向迭代
  int expected = 1;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    EXPECT_EQ(*it, expected++);
  }

  // 测试反向迭代
  expected = 5;
  for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
    EXPECT_EQ(*it, expected--);
  }
}

// 测试比较运算符
TEST(VectorTest, Comparison)
{
  sjkxq_stl::vector<int> vec1{1, 2, 3};
  sjkxq_stl::vector<int> vec2{1, 2, 3};
  sjkxq_stl::vector<int> vec3{1, 2, 4};
  sjkxq_stl::vector<int> vec4{1, 2};

  EXPECT_EQ(vec1, vec2);
  EXPECT_NE(vec1, vec3);
  EXPECT_NE(vec1, vec4);
  EXPECT_LT(vec1, vec3);
  EXPECT_GT(vec3, vec1);
  EXPECT_LE(vec1, vec2);
  EXPECT_GE(vec1, vec2);
}

// 测试复制和移动
TEST(VectorTest, CopyAndMove)
{
  sjkxq_stl::vector<std::string> vec1{"hello", "world"};

  // 测试复制构造
  sjkxq_stl::vector<std::string> vec2(vec1);
  EXPECT_EQ(vec1, vec2);

  // 测试移动构造
  sjkxq_stl::vector<std::string> vec3(std::move(vec2));
  EXPECT_EQ(vec1, vec3);
  EXPECT_TRUE(vec2.empty());  // vec2应该被移动走了

  // 测试复制赋值
  sjkxq_stl::vector<std::string> vec4;
  vec4 = vec1;
  EXPECT_EQ(vec1, vec4);

  // 测试移动赋值
  sjkxq_stl::vector<std::string> vec5;
  vec5 = std::move(vec4);
  EXPECT_EQ(vec1, vec5);
  EXPECT_TRUE(vec4.empty());  // vec4应该被移动走了
}

// 测试异常安全性
TEST(VectorTest, ExceptionSafety)
{
  sjkxq_stl::vector<int> vec;
  vec.reserve(5);  // 预分配空间，避免重新分配

  for (int i = 0; i < 5; ++i) {
    vec.push_back(i);
  }

  // 测试at()的边界检查
  EXPECT_THROW(vec.at(5), sjkxq_stl::out_of_range);
  EXPECT_THROW(vec.at(-1), sjkxq_stl::out_of_range);

  // 确保异常后vector仍然保持不变
  EXPECT_EQ(vec.size(), 5);
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(vec[i], i);
  }
}