#ifndef SJKXQ_STL_SET_HPP
#define SJKXQ_STL_SET_HPP

#include "common.hpp"
#include <functional>
#include <memory>
#include <set>

namespace sjkxq_stl
{

template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class set
{
public:
  // 类型定义
  using key_type        = Key;
  using value_type      = Key;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare     = Compare;
  using value_compare   = Compare;
  using allocator_type  = Allocator;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using pointer         = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;

  // 内部使用std::set作为实现
  using impl_type = std::set<Key, Compare, Allocator>;

  // 迭代器类型
  using iterator               = typename impl_type::const_iterator;
  using const_iterator         = typename impl_type::const_iterator;
  using reverse_iterator       = typename impl_type::const_reverse_iterator;
  using const_reverse_iterator = typename impl_type::const_reverse_iterator;

private:
  impl_type m_set;  // 底层实现

public:
  // 构造函数
  set() : m_set() {}

  explicit set(const Compare& comp, const Allocator& alloc = Allocator()) : m_set(comp, alloc) {}

  explicit set(const Allocator& alloc) : m_set(alloc) {}

  template <typename InputIt>
  set(InputIt          first,
      InputIt          last,
      const Compare&   comp  = Compare(),
      const Allocator& alloc = Allocator())
      : m_set(first, last, comp, alloc)
  {
  }

  template <typename InputIt>
  set(InputIt first, InputIt last, const Allocator& alloc) : m_set(first, last, Compare(), alloc)
  {
  }

  set(const set& other) : m_set(other.m_set) {}

  set(const set& other, const Allocator& alloc) : m_set(other.m_set, alloc) {}

  set(set&& other) noexcept : m_set(std::move(other.m_set)) {}

  set(set&& other, const Allocator& alloc) : m_set(std::move(other.m_set), alloc) {}

  set(std::initializer_list<value_type> init,
      const Compare&                    comp  = Compare(),
      const Allocator&                  alloc = Allocator())
      : m_set(init, comp, alloc)
  {
  }

  set(std::initializer_list<value_type> init, const Allocator& alloc)
      : m_set(init, Compare(), alloc)
  {
  }

  // 赋值运算符
  set& operator=(const set& other)
  {
    m_set = other.m_set;
    return *this;
  }

  set& operator=(set&& other) noexcept
  {
    m_set = std::move(other.m_set);
    return *this;
  }

  set& operator=(std::initializer_list<value_type> ilist)
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

  reverse_iterator rbegin() noexcept { return m_set.rbegin(); }

  const_reverse_iterator rbegin() const noexcept { return m_set.rbegin(); }

  const_reverse_iterator crbegin() const noexcept { return m_set.crbegin(); }

  reverse_iterator rend() noexcept { return m_set.rend(); }

  const_reverse_iterator rend() const noexcept { return m_set.rend(); }

  const_reverse_iterator crend() const noexcept { return m_set.crend(); }

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

  void swap(set& other) noexcept(std::is_nothrow_swappable_v<Compare>) { m_set.swap(other.m_set); }

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

  iterator lower_bound(const Key& key) { return m_set.lower_bound(key); }

  const_iterator lower_bound(const Key& key) const { return m_set.lower_bound(key); }

  template <typename K>
  iterator lower_bound(const K& x)
  {
    return m_set.lower_bound(x);
  }

  template <typename K>
  const_iterator lower_bound(const K& x) const
  {
    return m_set.lower_bound(x);
  }

  iterator upper_bound(const Key& key) { return m_set.upper_bound(key); }

  const_iterator upper_bound(const Key& key) const { return m_set.upper_bound(key); }

  template <typename K>
  iterator upper_bound(const K& x)
  {
    return m_set.upper_bound(x);
  }

  template <typename K>
  const_iterator upper_bound(const K& x) const
  {
    return m_set.upper_bound(x);
  }

  // 观察器
  key_compare key_comp() const { return m_set.key_comp(); }

  value_compare value_comp() const { return m_set.key_comp(); }

  // 比较运算符
  friend bool operator==(const set& lhs, const set& rhs) { return lhs.m_set == rhs.m_set; }

  friend bool operator!=(const set& lhs, const set& rhs) { return lhs.m_set != rhs.m_set; }

  friend bool operator<(const set& lhs, const set& rhs) { return lhs.m_set < rhs.m_set; }

  friend bool operator<=(const set& lhs, const set& rhs) { return lhs.m_set <= rhs.m_set; }

  friend bool operator>(const set& lhs, const set& rhs) { return lhs.m_set > rhs.m_set; }

  friend bool operator>=(const set& lhs, const set& rhs) { return lhs.m_set >= rhs.m_set; }
};

// 特化的swap函数
template <typename Key, typename Compare, typename Alloc>
void swap(set<Key, Compare, Alloc>& lhs,
          set<Key, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

}  // namespace sjkxq_stl

#endif  // SJKXQ_STL_SET_HPP