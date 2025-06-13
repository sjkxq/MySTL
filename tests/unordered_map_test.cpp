#include <gtest/gtest.h>
#include <sjkxq_stl/unordered_map.hpp>
#include <string>

// 测试默认构造函数和基本操作
TEST(UnorderedMapTest, DefaultConstructor) {
    sjkxq_stl::unordered_map<int, std::string> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0);
}

// 测试带桶数的构造函数
TEST(UnorderedMapTest, BucketConstructor) {
    sjkxq_stl::unordered_map<int, std::string> m(10);
    EXPECT_GE(m.bucket_count(), 10);
    EXPECT_TRUE(m.empty());
}

// 测试初始化列表构造函数
TEST(UnorderedMapTest, InitializerListConstructor) {
    sjkxq_stl::unordered_map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
    EXPECT_EQ(m.size(), 3);
    EXPECT_EQ(m[1], "one");
    EXPECT_EQ(m[2], "two");
    EXPECT_EQ(m[3], "three");
}

// 测试元素访问
TEST(UnorderedMapTest, ElementAccess) {
    sjkxq_stl::unordered_map<int, std::string> m;
    
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
TEST(UnorderedMapTest, Insertion) {
    sjkxq_stl::unordered_map<int, std::string> m;
    
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
    
    // 测试范围插入
    std::vector<std::pair<int, std::string>> v{{3, "three"}, {4, "four"}};
    m.insert(v.begin(), v.end());
    EXPECT_EQ(m.size(), 4);
}

// 测试删除操作
TEST(UnorderedMapTest, Erasure) {
    sjkxq_stl::unordered_map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
    
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
TEST(UnorderedMapTest, Lookup) {
    sjkxq_stl::unordered_map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
    
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

// 测试桶接口
TEST(UnorderedMapTest, BucketInterface) {
    sjkxq_stl::unordered_map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
    
    // 测试基本桶操作
    EXPECT_GT(m.bucket_count(), 0);
    EXPECT_GT(m.max_bucket_count(), m.bucket_count());
    
    // 测试特定桶的大小
    size_t bucket = m.bucket(1);
    EXPECT_GT(m.bucket_size(bucket), 0);
    
    // 测试桶迭代器
    bool found = false;
    for (auto it = m.begin(bucket); it != m.end(bucket); ++it) {
        if (it->first == 1) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

// 测试哈希策略
TEST(UnorderedMapTest, HashPolicy) {
    sjkxq_stl::unordered_map<int, std::string> m;
    
    // 测试负载因子
    EXPECT_GE(m.max_load_factor(), 0.0f);
    
    // 修改最大负载因子
    m.max_load_factor(0.75f);
    EXPECT_FLOAT_EQ(m.max_load_factor(), 0.75f);
    
    // 测试rehash
    size_t old_bucket_count = m.bucket_count();
    m.rehash(old_bucket_count * 2);
    EXPECT_GT(m.bucket_count(), old_bucket_count);
    
    // 测试reserve
    m.reserve(100);
    EXPECT_GE(m.bucket_count() * m.max_load_factor(), 100);
}

// 测试自定义哈希函数和相等比较
TEST(UnorderedMapTest, CustomHashAndEqual) {
    struct CustomHash {
        size_t operator()(int x) const { return std::hash<int>{}(x * 31); }
    };
    
    struct CustomEqual {
        bool operator()(int a, int b) const { return a == b; }
    };
    
    sjkxq_stl::unordered_map<int, std::string, CustomHash, CustomEqual> m;
    m.insert({1, "one"});
    m.insert({2, "two"});
    
    EXPECT_EQ(m[1], "one");
    EXPECT_EQ(m[2], "two");
}

// 测试移动语义
TEST(UnorderedMapTest, MoveSemantics) {
    sjkxq_stl::unordered_map<int, std::string> m1{{1, "one"}, {2, "two"}};
    
    // 测试移动构造
    sjkxq_stl::unordered_map<int, std::string> m2(std::move(m1));
    EXPECT_EQ(m2.size(), 2);
    EXPECT_TRUE(m1.empty());  // NOLINT - 访问移动后的对象是安全的
    
    // 测试移动赋值
    sjkxq_stl::unordered_map<int, std::string> m3;
    m3 = std::move(m2);
    EXPECT_EQ(m3.size(), 2);
    EXPECT_TRUE(m2.empty());  // NOLINT - 访问移动后的对象是安全的
}

// 测试复杂类型作为键
TEST(UnorderedMapTest, ComplexKey) {
    struct Point {
        int x, y;
        
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    struct PointHash {
        size_t operator()(const Point& p) const {
            return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
        }
    };
    
    sjkxq_stl::unordered_map<Point, std::string, PointHash> m;
    m.insert({{1, 2}, "point(1,2)"});
    m.insert({{3, 4}, "point(3,4)"});
    
    auto result1 = m[Point{1, 2}];
    auto result2 = m[Point{3, 4}];
    EXPECT_EQ(result1, std::string("point(1,2)"));
    EXPECT_EQ(result2, std::string("point(3,4)"));
}

// 测试异常安全性
TEST(UnorderedMapTest, ExceptionSafety) {
    sjkxq_stl::unordered_map<int, std::string> m{{1, "one"}, {2, "two"}};
    
    // at() 应该在键不存在时抛出异常
    EXPECT_THROW(m.at(3), std::out_of_range);
    
    // 确保异常后map保持不变
    EXPECT_EQ(m.size(), 2);
    EXPECT_EQ(m[1], "one");
    EXPECT_EQ(m[2], "two");
}

// 测试比较运算符
TEST(UnorderedMapTest, ComparisonOperators) {
    sjkxq_stl::unordered_map<int, std::string> m1{{1, "one"}, {2, "two"}};
    sjkxq_stl::unordered_map<int, std::string> m2{{1, "one"}, {2, "two"}};
    sjkxq_stl::unordered_map<int, std::string> m3{{1, "one"}, {2, "two"}, {3, "three"}};
    
    EXPECT_EQ(m1, m2);
    EXPECT_NE(m1, m3);
}

// 测试swap操作
TEST(UnorderedMapTest, Swap) {
    sjkxq_stl::unordered_map<int, std::string> m1{{1, "one"}, {2, "two"}};
    sjkxq_stl::unordered_map<int, std::string> m2{{3, "three"}, {4, "four"}, {5, "five"}};
    
    size_t size1 = m1.size();
    size_t size2 = m2.size();
    
    // 使用成员函数swap
    m1.swap(m2);
    
    EXPECT_EQ(m1.size(), size2);
    EXPECT_EQ(m2.size(), size1);
    EXPECT_TRUE(m1.contains(3));
    EXPECT_TRUE(m2.contains(1));
    
    // 使用非成员函数swap
    swap(m1, m2);
    
    EXPECT_EQ(m1.size(), size1);
    EXPECT_EQ(m2.size(), size2);
    EXPECT_TRUE(m1.contains(1));
    EXPECT_TRUE(m2.contains(3));
}