#ifndef SJKXQ_STL_UNORDERED_MAP_HPP
#define SJKXQ_STL_UNORDERED_MAP_HPP

#include "common.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

namespace sjkxq_stl
{

template <typename Key,
          typename T,
          typename Hash      = std::hash<Key>,
          typename KeyEqual  = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map
{
public:
  // 类型定义
  using key_type        = Key;
  using mapped_type     = T;
  using value_type      = std::pair<const Key, T>;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using hasher          = Hash;
  using key_equal       = KeyEqual;
  using allocator_type  = Allocator;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using pointer         = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

  // 内部使用std::unordered_map作为实现
  using impl_type = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

  // 迭代器类型
  using iterator             = typename impl_type::iterator;
  using const_iterator       = typename impl_type::const_iterator;
  using local_iterator       = typename impl_type::local_iterator;
  using const_local_iterator = typename impl_type::const_local_iterator;

private:
  impl_type m_map;  // 底层实现

public:
  // 构造函数
  unordered_map() : m_map() {}

  explicit unordered_map(size_type        bucket_count,
                         const Hash&      hash  = Hash(),
                         const KeyEqual&  equal = KeyEqual(),
                         const Allocator& alloc = Allocator())
      : m_map(bucket_count, hash, equal, alloc)
  {
  }

  unordered_map(size_type bucket_count, const Allocator& alloc)
      : m_map(bucket_count, Hash(), KeyEqual(), alloc)
  {
  }

  unordered_map(size_type bucket_count, const Hash& hash, const Allocator& alloc)
      : m_map(bucket_count, hash, KeyEqual(), alloc)
  {
  }

  explicit unordered_map(const Allocator& alloc) : m_map(alloc) {}

  template <typename InputIt>
  unordered_map(InputIt          first,
                InputIt          last,
                size_type        bucket_count = 0,
                const Hash&      hash         = Hash(),
                const KeyEqual&  equal        = KeyEqual(),
                const Allocator& alloc        = Allocator())
      : m_map(first, last, bucket_count, hash, equal, alloc)
  {
  }

  template <typename InputIt>
  unordered_map(InputIt first, InputIt last, size_type bucket_count, const Allocator& alloc)
      : m_map(first, last, bucket_count, Hash(), KeyEqual(), alloc)
  {
  }

  template <typename InputIt>
  unordered_map(
      InputIt first, InputIt last, size_type bucket_count, const Hash& hash, const Allocator& alloc)
      : m_map(first, last, bucket_count, hash, KeyEqual(), alloc)
  {
  }

  unordered_map(const unordered_map& other) : m_map(other.m_map) {}

  unordered_map(const unordered_map& other, const Allocator& alloc) : m_map(other.m_map, alloc) {}

  unordered_map(unordered_map&& other) noexcept : m_map(std::move(other.m_map)) {}

  unordered_map(unordered_map&& other, const Allocator& alloc)
      : m_map(std::move(other.m_map), alloc)
  {
  }

  unordered_map(std::initializer_list<value_type> init,
                size_type                         bucket_count = 0,
                const Hash&                       hash         = Hash(),
                const KeyEqual&                   equal        = KeyEqual(),
                const Allocator&                  alloc        = Allocator())
      : m_map(init, bucket_count, hash, equal, alloc)
  {
  }

  unordered_map(std::initializer_list<value_type> init,
                size_type                         bucket_count,
                const Allocator&                  alloc)
      : m_map(init, bucket_count, Hash(), KeyEqual(), alloc)
  {
  }

  unordered_map(std::initializer_list<value_type> init,
                size_type                         bucket_count,
                const Hash&                       hash,
                const Allocator&                  alloc)
      : m_map(init, bucket_count, hash, KeyEqual(), alloc)
  {
  }

  // 赋值运算符
  unordered_map& operator=(const unordered_map& other)
  {
    m_map = other.m_map;
    return *this;
  }

  unordered_map& operator=(unordered_map&& other) noexcept
  {
    m_map = std::move(other.m_map);
    return *this;
  }

  unordered_map& operator=(std::initializer_list<value_type> ilist)
  {
    m_map = ilist;
    return *this;
  }

  // 获取分配器
  allocator_type get_allocator() const noexcept { return m_map.get_allocator(); }

  // 元素访问
  T& at(const Key& key) { return m_map.at(key); }

  const T& at(const Key& key) const { return m_map.at(key); }

  T& operator[](const Key& key) { return m_map[key]; }

  T& operator[](Key&& key) { return m_map[std::move(key)]; }

  // 迭代器
  iterator begin() noexcept { return m_map.begin(); }

  const_iterator begin() const noexcept { return m_map.begin(); }

  const_iterator cbegin() const noexcept { return m_map.cbegin(); }

  iterator end() noexcept { return m_map.end(); }

  const_iterator end() const noexcept { return m_map.end(); }

  const_iterator cend() const noexcept { return m_map.cend(); }

  // 容量
  bool empty() const noexcept { return m_map.empty(); }

  size_type size() const noexcept { return m_map.size(); }

  size_type max_size() const noexcept { return m_map.max_size(); }

  // 修改器
  void clear() noexcept { m_map.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) { return m_map.insert(value); }

  std::pair<iterator, bool> insert(value_type&& value) { return m_map.insert(std::move(value)); }

  template <typename P>
  std::pair<iterator, bool> insert(P&& value)
  {
    return m_map.insert(std::forward<P>(value));
  }

  iterator insert(const_iterator hint, const value_type& value)
  {
    return m_map.insert(hint, value);
  }

  iterator insert(const_iterator hint, value_type&& value)
  {
    return m_map.insert(hint, std::move(value));
  }

  template <typename P>
  iterator insert(const_iterator hint, P&& value)
  {
    return m_map.insert(hint, std::forward<P>(value));
  }

  template <typename InputIt>
  void insert(InputIt first, InputIt last)
  {
    m_map.insert(first, last);
  }

  void insert(std::initializer_list<value_type> ilist) { m_map.insert(ilist); }

  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args)
  {
    return m_map.emplace(std::forward<Args>(args)...);
  }

  template <typename... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args)
  {
    return m_map.emplace_hint(hint, std::forward<Args>(args)...);
  }

  iterator erase(const_iterator pos) { return m_map.erase(pos); }

  iterator erase(const_iterator first, const_iterator last) { return m_map.erase(first, last); }

  size_type erase(const key_type& key) { return m_map.erase(key); }

  void swap(unordered_map& other) noexcept(std::is_nothrow_swappable_v<Hash>
                                           && std::is_nothrow_swappable_v<KeyEqual>)
  {
    m_map.swap(other.m_map);
  }

  // 查找
  size_type count(const Key& key) const { return m_map.count(key); }

  template <typename K>
  size_type count(const K& x) const
  {
    return m_map.count(x);
  }

  iterator find(const Key& key) { return m_map.find(key); }

  const_iterator find(const Key& key) const { return m_map.find(key); }

  template <typename K>
  iterator find(const K& x)
  {
    return m_map.find(x);
  }

  template <typename K>
  const_iterator find(const K& x) const
  {
    return m_map.find(x);
  }

  bool contains(const Key& key) const { return m_map.find(key) != m_map.end(); }

  template <typename K>
  bool contains(const K& x) const
  {
    return m_map.find(x) != m_map.end();
  }

  std::pair<iterator, iterator> equal_range(const Key& key) { return m_map.equal_range(key); }

  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const
  {
    return m_map.equal_range(key);
  }

  template <typename K>
  std::pair<iterator, iterator> equal_range(const K& x)
  {
    return m_map.equal_range(x);
  }

  template <typename K>
  std::pair<const_iterator, const_iterator> equal_range(const K& x) const
  {
    return m_map.equal_range(x);
  }

  // 桶接口
  size_type bucket_count() const noexcept { return m_map.bucket_count(); }

  size_type max_bucket_count() const noexcept { return m_map.max_bucket_count(); }

  size_type bucket_size(size_type n) const { return m_map.bucket_size(n); }

  size_type bucket(const Key& key) const { return m_map.bucket(key); }

  local_iterator begin(size_type n) { return m_map.begin(n); }

  const_local_iterator begin(size_type n) const { return m_map.begin(n); }

  const_local_iterator cbegin(size_type n) const { return m_map.cbegin(n); }

  local_iterator end(size_type n) { return m_map.end(n); }

  const_local_iterator end(size_type n) const { return m_map.end(n); }

  const_local_iterator cend(size_type n) const { return m_map.cend(n); }

  // 哈希策略
  float load_factor() const noexcept { return m_map.load_factor(); }

  float max_load_factor() const noexcept { return m_map.max_load_factor(); }

  void max_load_factor(float ml) { m_map.max_load_factor(ml); }

  void rehash(size_type count) { m_map.rehash(count); }

  void reserve(size_type count) { m_map.reserve(count); }

  // 观察器
  hasher hash_function() const { return m_map.hash_function(); }

  key_equal key_eq() const { return m_map.key_eq(); }

  // 比较运算符
  friend bool operator==(const unordered_map& lhs, const unordered_map& rhs)
  {
    return lhs.m_map == rhs.m_map;
  }

  friend bool operator!=(const unordered_map& lhs, const unordered_map& rhs)
  {
    return lhs.m_map != rhs.m_map;
  }
};

// 特化的swap函数
template <typename Key, typename T, typename Hash, typename KeyEqual, typename Alloc>
void swap(unordered_map<Key, T, Hash, KeyEqual, Alloc>& lhs,
          unordered_map<Key, T, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

}  // namespace sjkxq_stl

#endif  // SJKXQ_STL_UNORDERED_MAP_HPP