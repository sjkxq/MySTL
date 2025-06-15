#include <gtest/gtest.h>
#include <sjkxq_stl/set.hpp>
#include <string>

// 测试默认构造函数和基本操作
TEST(SetTest, DefaultConstructor)
{
  sjkxq_stl::set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

// 测试初始化列表构造函数
TEST(SetTest, InitializerListConstructor)
{
  sjkxq_stl::set<int> s{1, 2, 3, 4, 5};
  EXPECT_EQ(s.size(), 5);
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(3));
  EXPECT_TRUE(s.contains(5));
}

// 测试插入操作
TEST(SetTest, Insertion)
{
  sjkxq_stl::set<int> s;

  // 测试insert
  auto [it1, inserted1] = s.insert(42);
  EXPECT_TRUE(inserted1);
  EXPECT_EQ(*it1, 42);

  // 测试重复插入
  auto [it2, inserted2] = s.insert(42);
  EXPECT_FALSE(inserted2);
  EXPECT_EQ(*it2, 42);

  // 测试多个元素插入
  s.insert(10);
  s.insert(20);
  s.insert(30);
  EXPECT_EQ(s.size(), 4);

  // 测试范围插入
  std::vector<int> v{50, 60, 70};
  s.insert(v.begin(), v.end());
  EXPECT_EQ(s.size(), 7);

  // 测试初始化列表插入
  s.insert({80, 90, 100});
  EXPECT_EQ(s.size(), 10);

  // 测试emplace
  auto [it3, inserted3] = s.emplace(110);
  EXPECT_TRUE(inserted3);
  EXPECT_EQ(*it3, 110);
}

// 测试删除操作
TEST(SetTest, Erasure)
{
  sjkxq_stl::set<int> s{10, 20, 30, 40, 50};

  // 通过值删除
  EXPECT_EQ(s.erase(30), 1);
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.erase(60), 0);  // 删除不存在的元素

  // 通过迭代器删除
  auto it = s.find(20);
  s.erase(it);
  EXPECT_EQ(s.size(), 3);
  EXPECT_FALSE(s.contains(20));

  // 范围删除
  auto first = s.find(10);
  auto last  = s.find(50);
  s.erase(first, last);
  EXPECT_EQ(s.size(), 1);
  EXPECT_TRUE(s.contains(50));
}

// 测试查找操作
TEST(SetTest, Lookup)
{
  sjkxq_stl::set<int> s{10, 20, 30, 40, 50};

  // 测试find
  auto it = s.find(30);
  EXPECT_NE(it, s.end());
  EXPECT_EQ(*it, 30);

  EXPECT_EQ(s.find(60), s.end());

  // 测试count
  EXPECT_EQ(s.count(30), 1);
  EXPECT_EQ(s.count(60), 0);

  // 测试contains
  EXPECT_TRUE(s.contains(30));
  EXPECT_FALSE(s.contains(60));
}

// 测试范围操作
TEST(SetTest, RangeOperations)
{
  sjkxq_stl::set<int> s{10, 20, 30, 40, 50};

  // 测试lower_bound
  auto lower = s.lower_bound(25);
  EXPECT_EQ(*lower, 30);

  // 测试upper_bound
  auto upper = s.upper_bound(30);
  EXPECT_EQ(*upper, 40);

  // 测试equal_range
  auto [range_begin, range_end] = s.equal_range(30);
  EXPECT_EQ(*range_begin, 30);
  EXPECT_EQ(*range_end, 40);
}

// 测试迭代器
TEST(SetTest, Iterators)
{
  sjkxq_stl::set<int> s{10, 20, 30, 40, 50};

  // 测试正向迭代
  int expected_val = 10;
  for (const auto& val : s) {
    EXPECT_EQ(val, expected_val);
    expected_val += 10;
  }

  // 测试反向迭代
  expected_val = 50;
  for (auto it = s.rbegin(); it != s.rend(); ++it) {
    EXPECT_EQ(*it, expected_val);
    expected_val -= 10;
  }
}

// 测试比较运算符
TEST(SetTest, Comparison)
{
  sjkxq_stl::set<int> s1{10, 20, 30};
  sjkxq_stl::set<int> s2{10, 20, 30};
  sjkxq_stl::set<int> s3{10, 20, 30, 40};
  sjkxq_stl::set<int> s4{10, 20, 25};

  EXPECT_EQ(s1, s2);
  EXPECT_NE(s1, s3);
  EXPECT_LT(s1, s3);
  EXPECT_LE(s1, s2);
  EXPECT_GT(s3, s1);
  EXPECT_GE(s2, s1);

  EXPECT_NE(s1, s4);
  EXPECT_GT(s1, s4);  // 因为30 > 25
}

// 测试清空操作
TEST(SetTest, Clear)
{
  sjkxq_stl::set<int> s{10, 20, 30, 40, 50};
  EXPECT_FALSE(s.empty());

  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

// 测试自定义比较器
TEST(SetTest, CustomComparator)
{
  struct ReverseCompare {
    bool operator()(int a, int b) const { return a > b; }
  };

  sjkxq_stl::set<int, ReverseCompare> s{10, 20, 30, 40, 50};

  // 验证反向排序
  int expected_val = 50;
  for (const auto& val : s) {
    EXPECT_EQ(val, expected_val);
    expected_val -= 10;
  }
}

// 测试移动语义
TEST(SetTest, MoveSemantics)
{
  sjkxq_stl::set<int> s1{10, 20, 30};

  // 测试移动构造
  sjkxq_stl::set<int> s2(std::move(s1));
  EXPECT_EQ(s2.size(), 3);
  EXPECT_TRUE(s1.empty());  // NOLINT - 访问移动后的对象是安全的

  // 测试移动赋值
  sjkxq_stl::set<int> s3;
  s3 = std::move(s2);
  EXPECT_EQ(s3.size(), 3);
  EXPECT_TRUE(s2.empty());  // NOLINT - 访问移动后的对象是安全的
}

// 测试复杂类型
TEST(SetTest, ComplexTypes)
{
  struct Person {
    std::string name;
    int         age;

    bool operator<(const Person& other) const { return name < other.name; }

    bool operator==(const Person& other) const { return name == other.name && age == other.age; }
  };

  sjkxq_stl::set<Person> people;
  people.insert({"Alice", 25});
  people.insert({"Bob", 30});
  people.insert({"Charlie", 35});

  EXPECT_EQ(people.size(), 3);

  auto it = people.find({"Bob", 30});
  EXPECT_NE(it, people.end());
  EXPECT_EQ(it->name, "Bob");
  EXPECT_EQ(it->age, 30);

  // 测试相同名字但不同年龄的情况
  people.insert({"Alice", 26});  // 不应该插入，因为比较只基于名字
  EXPECT_EQ(people.size(), 3);
}

// 测试swap操作
TEST(SetTest, Swap)
{
  sjkxq_stl::set<int> s1{1, 2, 3};
  sjkxq_stl::set<int> s2{4, 5, 6, 7};

  // 使用成员函数swap
  s1.swap(s2);

  EXPECT_EQ(s1.size(), 4);
  EXPECT_TRUE(s1.contains(4));
  EXPECT_TRUE(s1.contains(7));

  EXPECT_EQ(s2.size(), 3);
  EXPECT_TRUE(s2.contains(1));
  EXPECT_TRUE(s2.contains(3));

  // 使用非成员函数swap
  swap(s1, s2);

  EXPECT_EQ(s1.size(), 3);
  EXPECT_TRUE(s1.contains(1));
  EXPECT_TRUE(s1.contains(3));

  EXPECT_EQ(s2.size(), 4);
  EXPECT_TRUE(s2.contains(4));
  EXPECT_TRUE(s2.contains(7));
}