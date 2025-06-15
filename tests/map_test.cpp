#include <gtest/gtest.h>
#include <sjkxq_stl/map.hpp>
#include <string>

// 测试默认构造函数和基本操作
TEST(MapTest, DefaultConstructor)
{
  sjkxq_stl::map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

// 测试初始化列表构造函数
TEST(MapTest, InitializerListConstructor)
{
  sjkxq_stl::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
  EXPECT_EQ(m[3], "three");
}

// 测试元素访问
TEST(MapTest, ElementAccess)
{
  sjkxq_stl::map<int, std::string> m;

  // 测试operator[]
  m[1] = "one";
  EXPECT_EQ(m[1], "one");

  // 测试at()
  EXPECT_EQ(m.at(1), "one");
  EXPECT_THROW(m.at(2), std::out_of_range);

  // 测试operator[]创建新元素
  EXPECT_EQ(m[2], "");  // 创建新元素，默认构造
  EXPECT_EQ(m.size(), 2);
}

// 测试插入操作
TEST(MapTest, Insertion)
{
  sjkxq_stl::map<int, std::string> m;

  // 测试insert
  auto [it1, inserted1] = m.insert({1, "one"});
  EXPECT_TRUE(inserted1);
  EXPECT_EQ(it1->first, 1);
  EXPECT_EQ(it1->second, "one");

  // 测试重复插入
  auto [it2, inserted2] = m.insert({1, "another one"});
  EXPECT_FALSE(inserted2);
  EXPECT_EQ(m[1], "one");  // 原值未改变

  // 测试emplace
  auto [it3, inserted3] = m.emplace(2, "two");
  EXPECT_TRUE(inserted3);
  EXPECT_EQ(it3->first, 2);
  EXPECT_EQ(it3->second, "two");
}

// 测试删除操作
TEST(MapTest, Erasure)
{
  sjkxq_stl::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};

  // 通过键删除
  EXPECT_EQ(m.erase(2), 1);
  EXPECT_EQ(m.size(), 2);
  EXPECT_EQ(m.erase(4), 0);  // 删除不存在的元素

  // 通过迭代器删除
  auto it = m.find(1);
  m.erase(it);
  EXPECT_EQ(m.size(), 1);
  EXPECT_FALSE(m.contains(1));
}

// 测试查找操作
TEST(MapTest, Lookup)
{
  sjkxq_stl::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};

  // 测试find
  auto it = m.find(2);
  EXPECT_NE(it, m.end());
  EXPECT_EQ(it->second, "two");

  EXPECT_EQ(m.find(4), m.end());

  // 测试count
  EXPECT_EQ(m.count(2), 1);
  EXPECT_EQ(m.count(4), 0);

  // 测试contains
  EXPECT_TRUE(m.contains(2));
  EXPECT_FALSE(m.contains(4));
}

// 测试范围操作
TEST(MapTest, RangeOperations)
{
  sjkxq_stl::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};

  // 测试lower_bound
  auto lower = m.lower_bound(2);
  EXPECT_EQ(lower->first, 2);

  // 测试upper_bound
  auto upper = m.upper_bound(2);
  EXPECT_EQ(upper->first, 3);

  // 测试equal_range
  auto [range_begin, range_end] = m.equal_range(2);
  EXPECT_EQ(range_begin->first, 2);
  EXPECT_EQ(range_end->first, 3);
}

// 测试迭代器
TEST(MapTest, Iterators)
{
  sjkxq_stl::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};

  // 测试正向迭代
  int expected_key = 1;
  for (const auto& [key, value] : m) {
    EXPECT_EQ(key, expected_key++);
  }

  // 测试反向迭代
  expected_key = 3;
  for (auto it = m.rbegin(); it != m.rend(); ++it) {
    EXPECT_EQ(it->first, expected_key--);
  }
}

// 测试比较运算符
TEST(MapTest, Comparison)
{
  sjkxq_stl::map<int, std::string> m1{{1, "one"}, {2, "two"}};
  sjkxq_stl::map<int, std::string> m2{{1, "one"}, {2, "two"}};
  sjkxq_stl::map<int, std::string> m3{{1, "one"}, {2, "two"}, {3, "three"}};

  EXPECT_EQ(m1, m2);
  EXPECT_NE(m1, m3);
  EXPECT_LT(m1, m3);
  EXPECT_LE(m1, m2);
  EXPECT_GT(m3, m1);
  EXPECT_GE(m2, m1);
}

// 测试清空操作
TEST(MapTest, Clear)
{
  sjkxq_stl::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
  EXPECT_FALSE(m.empty());

  m.clear();
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

// 测试自定义比较器
TEST(MapTest, CustomComparator)
{
  struct ReverseCompare {
    bool operator()(int a, int b) const { return a > b; }
  };

  sjkxq_stl::map<int, std::string, ReverseCompare> m{{1, "one"}, {2, "two"}, {3, "three"}};

  // 验证反向排序
  int expected_key = 3;
  for (const auto& [key, value] : m) {
    EXPECT_EQ(key, expected_key--);
  }
}

// 测试移动语义
TEST(MapTest, MoveSemantics)
{
  sjkxq_stl::map<int, std::string> m1{{1, "one"}, {2, "two"}};

  // 测试移动构造
  sjkxq_stl::map<int, std::string> m2(std::move(m1));
  EXPECT_EQ(m2.size(), 2);
  EXPECT_TRUE(m1.empty());  // NOLINT - 访问移动后的对象是安全的

  // 测试移动赋值
  sjkxq_stl::map<int, std::string> m3;
  m3 = std::move(m2);
  EXPECT_EQ(m3.size(), 2);
  EXPECT_TRUE(m2.empty());  // NOLINT - 访问移动后的对象是安全的
}

// 测试异常安全性
TEST(MapTest, ExceptionSafety)
{
  sjkxq_stl::map<int, std::string> m{{1, "one"}, {2, "two"}};

  // at() 应该在键不存在时抛出异常
  EXPECT_THROW(m.at(3), std::out_of_range);

  // 确保异常后map保持不变
  EXPECT_EQ(m.size(), 2);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
}