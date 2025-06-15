#ifndef SJKXQ_STL_UNORDERED_SET_HPP
#define SJKXQ_STL_UNORDERED_SET_HPP

#include "common.hpp"
#include <functional>
#include <memory>
#include <unordered_set>

namespace sjkxq_stl
{

template <typename Key,
          typename Hash      = std::hash<Key>,
          typename KeyEqual  = std::equal_to<Key>,
          typename Allocator = std::allocator<Key>>
class unordered_set
{
public:
  // 类型定义
  using key_type        = Key;
  using value_type      = Key;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using hasher          = Hash;
  using key_equal       = KeyEqual;
  using allocator_type  = Allocator;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using pointer         = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

  // 内部使用std::unordered_set作为实现
  using impl_type = std::unordered_set<Key, Hash, KeyEqual, Allocator>;

  // 迭代器类型
  using iterator             = typename impl_type::iterator;
  using const_iterator       = typename impl_type::const_iterator;
  using local_iterator       = typename impl_type::local_iterator;
  using const_local_iterator = typename impl_type::const_local_iterator;

private:
  impl_type m_set;  // 底层实现

public:
  // 构造函数
  unordered_set() : m_set() {}

  explicit unordered_set(size_type        bucket_count,
                         const Hash&      hash  = Hash(),
                         const KeyEqual&  equal = KeyEqual(),
                         const Allocator& alloc = Allocator())
      : m_set(bucket_count, hash, equal, alloc)
  {
  }

  unordered_set(size_type bucket_count, const Allocator& alloc)
      : m_set(bucket_count, Hash(), KeyEqual(), alloc)
  {
  }

  unordered_set(size_type bucket_count, const Hash& hash, const Allocator& alloc)
      : m_set(bucket_count, hash, KeyEqual(), alloc)
  {
  }

  explicit unordered_set(const Allocator& alloc) : m_set(alloc) {}

  template <typename InputIt>
  unordered_set(InputIt          first,
                InputIt          last,
                size_type        bucket_count = 0,
                const Hash&      hash         = Hash(),
                const KeyEqual&  equal        = KeyEqual(),
                const Allocator& alloc        = Allocator())
      : m_set(first, last, bucket_count, hash, equal, alloc)
  {
  }

  template <typename InputIt>
  unordered_set(InputIt first, InputIt last, size_type bucket_count, const Allocator& alloc)
      : m_set(first, last, bucket_count, Hash(), KeyEqual(), alloc)
  {
  }

  template <typename InputIt>
  unordered_set(
      InputIt first, InputIt last, size_type bucket_count, const Hash& hash, const Allocator& alloc)
      : m_set(first, last, bucket_count, hash, KeyEqual(), alloc)
  {
  }

  unordered_set(const unordered_set& other) : m_set(other.m_set) {}

  unordered_set(const unordered_set& other, const Allocator& alloc) : m_set(other.m_set, alloc) {}

  unordered_set(unordered_set&& other) noexcept : m_set(std::move(other.m_set)) {}

  unordered_set(unordered_set&& other, const Allocator& alloc)
      : m_set(std::move(other.m_set), alloc)
  {
  }

  unordered_set(std::initializer_list<value_type> init,
                size_type                         bucket_count = 0,
                const Hash&                       hash         = Hash(),
                const KeyEqual&                   equal        = KeyEqual(),
                const Allocator&                  alloc        = Allocator())
      : m_set(init, bucket_count, hash, equal, alloc)
  {
  }

  unordered_set(std::initializer_list<value_type> init,
                size_type                         bucket_count,
                const Allocator&                  alloc)
      : m_set(init, bucket_count, Hash(), KeyEqual(), alloc)
  {
  }

  unordered_set(std::initializer_list<value_type> init,
                size_type                         bucket_count,
                const Hash&                       hash,
                const Allocator&                  alloc)
      : m_set(init, bucket_count, hash, KeyEqual(), alloc)
  {
  }

  // 赋值运算符
  unordered_set& operator=(const unordered_set& other)
  {
    m_set = other.m_set;
    return *this;
  }

  unordered_set& operator=(unordered_set&& other) noexcept
  {
    m_set = std::move(other.m_set);
    return *this;
  }

  unordered_set& operator=(std::initializer_list<value_type> ilist)
  {
    m_set = ilist;
    return *this;
  }

  // 获取分配器
  allocator_type get_allocator() const noexcept { return m_set.get_allocator(); }

  // 迭代器
  iterator begin() noexcept { return m_set.begin(); }

  const_iterator begin() const noexcept { return m_set.begin(); }

  const_iterator cbegin() const noexcept { return m_set.cbegin(); }

  iterator end() noexcept { return m_set.end(); }

  const_iterator end() const noexcept { return m_set.end(); }

  const_iterator cend() const noexcept { return m_set.cend(); }

  // 容量
  bool empty() const noexcept { return m_set.empty(); }

  size_type size() const noexcept { return m_set.size(); }

  size_type max_size() const noexcept { return m_set.max_size(); }

  // 修改器
  void clear() noexcept { m_set.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) { return m_set.insert(value); }

  std::pair<iterator, bool> insert(value_type&& value) { return m_set.insert(std::move(value)); }

  iterator insert(const_iterator hint, const value_type& value)
  {
    return m_set.insert(hint, value);
  }

  iterator insert(const_iterator hint, value_type&& value)
  {
    return m_set.insert(hint, std::move(value));
  }

  template <typename InputIt>
  void insert(InputIt first, InputIt last)
  {
    m_set.insert(first, last);
  }

  void insert(std::initializer_list<value_type> ilist) { m_set.insert(ilist); }

  template <typename... Args>
  std::pair<iterator, bool> emplace(Args&&... args)
  {
    return m_set.emplace(std::forward<Args>(args)...);
  }

  template <typename... Args>
  iterator emplace_hint(const_iterator hint, Args&&... args)
  {
    return m_set.emplace_hint(hint, std::forward<Args>(args)...);
  }

  iterator erase(const_iterator pos) { return m_set.erase(pos); }

  iterator erase(const_iterator first, const_iterator last) { return m_set.erase(first, last); }

  size_type erase(const key_type& key) { return m_set.erase(key); }

  void swap(unordered_set& other) noexcept(std::is_nothrow_swappable_v<Hash>
                                           && std::is_nothrow_swappable_v<KeyEqual>)
  {
    m_set.swap(other.m_set);
  }

  // 查找
  size_type count(const Key& key) const { return m_set.count(key); }

  template <typename K>
  size_type count(const K& x) const
  {
    return m_set.count(x);
  }

  iterator find(const Key& key) { return m_set.find(key); }

  const_iterator find(const Key& key) const { return m_set.find(key); }

  template <typename K>
  iterator find(const K& x)
  {
    return m_set.find(x);
  }

  template <typename K>
  const_iterator find(const K& x) const
  {
    return m_set.find(x);
  }

  bool contains(const Key& key) const { return m_set.find(key) != m_set.end(); }

  template <typename K>
  bool contains(const K& x) const
  {
    return m_set.find(x) != m_set.end();
  }

  std::pair<iterator, iterator> equal_range(const Key& key) { return m_set.equal_range(key); }

  std::pair<const_iterator, const_iterator> equal_range(const Key& key) const
  {
    return m_set.equal_range(key);
  }

  template <typename K>
  std::pair<iterator, iterator> equal_range(const K& x)
  {
    return m_set.equal_range(x);
  }

  template <typename K>
  std::pair<const_iterator, const_iterator> equal_range(const K& x) const
  {
    return m_set.equal_range(x);
  }

  // 桶接口
  size_type bucket_count() const noexcept { return m_set.bucket_count(); }

  size_type max_bucket_count() const noexcept { return m_set.max_bucket_count(); }

  size_type bucket_size(size_type n) const { return m_set.bucket_size(n); }

  size_type bucket(const Key& key) const { return m_set.bucket(key); }

  local_iterator begin(size_type n) { return m_set.begin(n); }

  const_local_iterator begin(size_type n) const { return m_set.begin(n); }

  const_local_iterator cbegin(size_type n) const { return m_set.cbegin(n); }

  local_iterator end(size_type n) { return m_set.end(n); }

  const_local_iterator end(size_type n) const { return m_set.end(n); }

  const_local_iterator cend(size_type n) const { return m_set.cend(n); }

  // 哈希策略
  float load_factor() const noexcept { return m_set.load_factor(); }

  float max_load_factor() const noexcept { return m_set.max_load_factor(); }

  void max_load_factor(float ml) { m_set.max_load_factor(ml); }

  void rehash(size_type count) { m_set.rehash(count); }

  void reserve(size_type count) { m_set.reserve(count); }

  // 观察器
  hasher hash_function() const { return m_set.hash_function(); }

  key_equal key_eq() const { return m_set.key_eq(); }

  // 比较运算符
  friend bool operator==(const unordered_set& lhs, const unordered_set& rhs)
  {
    return lhs.m_set == rhs.m_set;
  }

  friend bool operator!=(const unordered_set& lhs, const unordered_set& rhs)
  {
    return lhs.m_set != rhs.m_set;
  }
};

// 特化的swap函数
template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
void swap(unordered_set<Key, Hash, KeyEqual, Alloc>& lhs,
          unordered_set<Key, Hash, KeyEqual, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

}  // namespace sjkxq_stl

#endif  // SJKXQ_STL_UNORDERED_SET_HPP