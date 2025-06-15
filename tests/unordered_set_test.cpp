#include <gtest/gtest.h>
#include <sjkxq_stl/unordered_set.hpp>
#include <string>

// 测试默认构造函数和基本操作
TEST(UnorderedSetTest, DefaultConstructor)
{
  sjkxq_stl::unordered_set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

// 测试带桶数的构造函数
TEST(UnorderedSetTest, BucketConstructor)
{
  sjkxq_stl::unordered_set<int> s(10);
  EXPECT_GE(s.bucket_count(), 10);
  EXPECT_TRUE(s.empty());
}

// 测试初始化列表构造函数
TEST(UnorderedSetTest, InitializerListConstructor)
{
  sjkxq_stl::unordered_set<int> s{1, 2, 3};
  EXPECT_EQ(s.size(), 3);
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(2));
  EXPECT_TRUE(s.contains(3));
}

// 测试插入操作
TEST(UnorderedSetTest, Insertion)
{
  sjkxq_stl::unordered_set<int> s;

  // 测试insert
  auto [it1, inserted1] = s.insert(1);
  EXPECT_TRUE(inserted1);
  EXPECT_EQ(*it1, 1);

  // 测试重复插入
  auto [it2, inserted2] = s.insert(1);
  EXPECT_FALSE(inserted2);
  EXPECT_EQ(*it2, 1);  // 迭代器指向已存在的元素

  // 测试emplace
  auto [it3, inserted3] = s.emplace(2);
  EXPECT_TRUE(inserted3);
  EXPECT_EQ(*it3, 2);

  // 测试范围插入
  std::vector<int> v{3, 4};
  s.insert(v.begin(), v.end());
  EXPECT_EQ(s.size(), 4);
}

// 测试删除操作
TEST(UnorderedSetTest, Erasure)
{
  sjkxq_stl::unordered_set<int> s{1, 2, 3};

  // 通过值删除
  EXPECT_EQ(s.erase(2), 1);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.erase(4), 0);  // 删除不存在的元素

  // 通过迭代器删除
  auto it = s.find(1);
  s.erase(it);
  EXPECT_EQ(s.size(), 1);
  EXPECT_FALSE(s.contains(1));
}

// 测试查找操作
TEST(UnorderedSetTest, Lookup)
{
  sjkxq_stl::unordered_set<int> s{1, 2, 3};

  // 测试find
  auto it = s.find(2);
  EXPECT_NE(it, s.end());
  EXPECT_EQ(*it, 2);

  EXPECT_EQ(s.find(4), s.end());

  // 测试count
  EXPECT_EQ(s.count(2), 1);
  EXPECT_EQ(s.count(4), 0);

  // 测试contains
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(4));
}

// 测试桶接口
TEST(UnorderedSetTest, BucketInterface)
{
  sjkxq_stl::unordered_set<int> s{1, 2, 3};

  // 测试基本桶操作
  EXPECT_GT(s.bucket_count(), 0);
  EXPECT_GT(s.max_bucket_count(), s.bucket_count());

  // 测试特定桶的大小
  size_t bucket = s.bucket(1);
  EXPECT_GT(s.bucket_size(bucket), 0);

  // 测试桶迭代器
  bool found = false;
  for (auto it = s.begin(bucket); it != s.end(bucket); ++it) {
    if (*it == 1) {
      found = true;
      break;
    }
  }
  EXPECT_TRUE(found);
}

// 测试哈希策略
TEST(UnorderedSetTest, HashPolicy)
{
  sjkxq_stl::unordered_set<int> s;

  // 测试负载因子
  EXPECT_GE(s.max_load_factor(), 0.0f);

  // 修改最大负载因子
  s.max_load_factor(0.75f);
  EXPECT_FLOAT_EQ(s.max_load_factor(), 0.75f);

  // 测试rehash
  size_t old_bucket_count = s.bucket_count();
  s.rehash(old_bucket_count * 2);
  EXPECT_GT(s.bucket_count(), old_bucket_count);

  // 测试reserve
  s.reserve(100);
  EXPECT_GE(s.bucket_count() * s.max_load_factor(), 100);
}

// 测试自定义哈希函数和相等比较
TEST(UnorderedSetTest, CustomHashAndEqual)
{
  struct CustomHash {
    size_t operator()(int x) const { return std::hash<int>{}(x * 31); }
  };

  struct CustomEqual {
    bool operator()(int a, int b) const { return a == b; }
  };

  sjkxq_stl::unordered_set<int, CustomHash, CustomEqual> s;
  s.insert(1);
  s.insert(2);

  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(2));
}

// 测试移动语义
TEST(UnorderedSetTest, MoveSemantics)
{
  sjkxq_stl::unordered_set<int> s1{1, 2};

  // 测试移动构造
  sjkxq_stl::unordered_set<int> s2(std::move(s1));
  EXPECT_EQ(s2.size(), 2);
  EXPECT_TRUE(s1.empty());  // NOLINT - 访问移动后的对象是安全的

  // 测试移动赋值
  sjkxq_stl::unordered_set<int> s3;
  s3 = std::move(s2);
  EXPECT_EQ(s3.size(), 2);
  EXPECT_TRUE(s2.empty());  // NOLINT - 访问移动后的对象是安全的
}

// 测试复杂类型作为键
TEST(UnorderedSetTest, ComplexKey)
{
  struct Point {
    int x, y;

    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
  };

  struct PointHash {
    size_t operator()(const Point& p) const
    {
      return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
    }
  };

  sjkxq_stl::unordered_set<Point, PointHash> s;
  s.insert({1, 2});
  s.insert({3, 4});

  EXPECT_TRUE(s.contains(Point{1, 2}));
  EXPECT_TRUE(s.contains(Point{3, 4}));
  EXPECT_FALSE(s.contains(Point{5, 6}));
}

// 测试比较运算符
TEST(UnorderedSetTest, ComparisonOperators)
{
  sjkxq_stl::unordered_set<int> s1{1, 2};
  sjkxq_stl::unordered_set<int> s2{1, 2};
  sjkxq_stl::unordered_set<int> s3{1, 2, 3};

  EXPECT_EQ(s1, s2);
  EXPECT_NE(s1, s3);
}

// 测试swap操作
TEST(UnorderedSetTest, Swap)
{
  sjkxq_stl::unordered_set<int> s1{1, 2};
  sjkxq_stl::unordered_set<int> s2{3, 4, 5};

  size_t size1 = s1.size();
  size_t size2 = s2.size();

  // 使用成员函数swap
  s1.swap(s2);

  EXPECT_EQ(s1.size(), size2);
  EXPECT_EQ(s2.size(), size1);
  EXPECT_TRUE(s1.contains(3));
  EXPECT_TRUE(s2.contains(1));

  // 使用非成员函数swap
  swap(s1, s2);

  EXPECT_EQ(s1.size(), size1);
  EXPECT_EQ(s2.size(), size2);
  EXPECT_TRUE(s1.contains(1));
  EXPECT_TRUE(s2.contains(3));
}

// 测试迭代器
TEST(UnorderedSetTest, Iterators)
{
  sjkxq_stl::unordered_set<int> s{1, 2, 3};

  // 测试迭代器遍历
  std::vector<int> values;
  for (auto it = s.begin(); it != s.end(); ++it) {
    values.push_back(*it);
  }

  EXPECT_EQ(values.size(), 3);
  EXPECT_TRUE(std::find(values.begin(), values.end(), 1) != values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), 2) != values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), 3) != values.end());

  // 测试const迭代器
  const sjkxq_stl::unordered_set<int>& cs = s;
  std::vector<int> const_values;
  for (auto it = cs.begin(); it != cs.end(); ++it) {
    const_values.push_back(*it);
  }

  EXPECT_EQ(const_values.size(), 3);
}

// 测试范围for循环
TEST(UnorderedSetTest, RangeForLoop)
{
  sjkxq_stl::unordered_set<int> s{1, 2, 3};

  std::vector<int> values;
  for (const auto& value : s) {
    values.push_back(value);
  }

  EXPECT_EQ(values.size(), 3);
  EXPECT_TRUE(std::find(values.begin(), values.end(), 1) != values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), 2) != values.end());
  EXPECT_TRUE(std::find(values.begin(), values.end(), 3) != values.end());
}

// 测试clear操作
TEST(UnorderedSetTest, Clear)
{
  sjkxq_stl::unordered_set<int> s{1, 2, 3};
  EXPECT_EQ(s.size(), 3);

  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

// 测试emplace操作
TEST(UnorderedSetTest, Emplace)
{
  sjkxq_stl::unordered_set<std::string> s;

  // 使用emplace直接构造元素
  auto [it1, inserted1] = s.emplace("hello");
  EXPECT_TRUE(inserted1);
  EXPECT_EQ(*it1, "hello");

  // 使用emplace_hint提供位置提示
  auto hint = s.find("hello");
  auto it2 = s.emplace_hint(hint, "world");
  EXPECT_EQ(*it2, "world");
  EXPECT_EQ(s.size(), 2);
}