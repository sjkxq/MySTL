#ifndef SJKXQ_STL_MAP_HPP
#define SJKXQ_STL_MAP_HPP

#include "common.hpp"
#include <map>
#include <functional>
#include <memory>

namespace sjkxq_stl {

template <
    typename Key,
    typename T,
    typename Compare = std::less<Key>,
    typename Allocator = std::allocator<std::pair<const Key, T>>
>
class map {
public:
    // 类型定义
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    
    // 内部使用std::map作为实现
    using impl_type = std::map<Key, T, Compare, Allocator>;
    
    // 迭代器类型
    using iterator = typename impl_type::iterator;
    using const_iterator = typename impl_type::const_iterator;
    using reverse_iterator = typename impl_type::reverse_iterator;
    using const_reverse_iterator = typename impl_type::const_reverse_iterator;
    
    // 值比较器
    class value_compare {
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const {
            return comp(lhs.first, rhs.first);
        }
        friend class map;
    };
    
private:
    impl_type m_map; // 底层实现
    
public:
    // 构造函数
    map() : m_map() {}
    
    explicit map(const Compare& comp, const Allocator& alloc = Allocator())
        : m_map(comp, alloc) {}
    
    explicit map(const Allocator& alloc)
        : m_map(alloc) {}
    
    template <typename InputIt>
    map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : m_map(first, last, comp, alloc) {}
    
    template <typename InputIt>
    map(InputIt first, InputIt last, const Allocator& alloc)
        : m_map(first, last, Compare(), alloc) {}
    
    map(const map& other) : m_map(other.m_map) {}
    
    map(const map& other, const Allocator& alloc)
        : m_map(other.m_map, alloc) {}
    
    map(map&& other) noexcept : m_map(std::move(other.m_map)) {}
    
    map(map&& other, const Allocator& alloc)
        : m_map(std::move(other.m_map), alloc) {}
    
    map(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : m_map(init, comp, alloc) {}
    
    map(std::initializer_list<value_type> init, const Allocator& alloc)
        : m_map(init, Compare(), alloc) {}
    
    // 赋值运算符
    map& operator=(const map& other) {
        m_map = other.m_map;
        return *this;
    }
    
    map& operator=(map&& other) noexcept {
        m_map = std::move(other.m_map);
        return *this;
    }
    
    map& operator=(std::initializer_list<value_type> ilist) {
        m_map = ilist;
        return *this;
    }
    
    // 获取分配器
    allocator_type get_allocator() const noexcept {
        return m_map.get_allocator();
    }
    
    // 元素访问
    T& at(const Key& key) {
        return m_map.at(key);
    }
    
    const T& at(const Key& key) const {
        return m_map.at(key);
    }
    
    T& operator[](const Key& key) {
        return m_map[key];
    }
    
    T& operator[](Key&& key) {
        return m_map[std::move(key)];
    }
    
    // 迭代器
    iterator begin() noexcept {
        return m_map.begin();
    }
    
    const_iterator begin() const noexcept {
        return m_map.begin();
    }
    
    const_iterator cbegin() const noexcept {
        return m_map.cbegin();
    }
    
    iterator end() noexcept {
        return m_map.end();
    }
    
    const_iterator end() const noexcept {
        return m_map.end();
    }
    
    const_iterator cend() const noexcept {
        return m_map.cend();
    }
    
    reverse_iterator rbegin() noexcept {
        return m_map.rbegin();
    }
    
    const_reverse_iterator rbegin() const noexcept {
        return m_map.rbegin();
    }
    
    const_reverse_iterator crbegin() const noexcept {
        return m_map.crbegin();
    }
    
    reverse_iterator rend() noexcept {
        return m_map.rend();
    }
    
    const_reverse_iterator rend() const noexcept {
        return m_map.rend();
    }
    
    const_reverse_iterator crend() const noexcept {
        return m_map.crend();
    }
    
    // 容量
    bool empty() const noexcept {
        return m_map.empty();
    }
    
    size_type size() const noexcept {
        return m_map.size();
    }
    
    size_type max_size() const noexcept {
        return m_map.max_size();
    }
    
    // 修改器
    void clear() noexcept {
        m_map.clear();
    }
    
    std::pair<iterator, bool> insert(const value_type& value) {
        return m_map.insert(value);
    }
    
    std::pair<iterator, bool> insert(value_type&& value) {
        return m_map.insert(std::move(value));
    }
    
    template <typename P>
    std::pair<iterator, bool> insert(P&& value) {
        return m_map.insert(std::forward<P>(value));
    }
    
    iterator insert(const_iterator hint, const value_type& value) {
        return m_map.insert(hint, value);
    }
    
    iterator insert(const_iterator hint, value_type&& value) {
        return m_map.insert(hint, std::move(value));
    }
    
    template <typename P>
    iterator insert(const_iterator hint, P&& value) {
        return m_map.insert(hint, std::forward<P>(value));
    }
    
    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        m_map.insert(first, last);
    }
    
    void insert(std::initializer_list<value_type> ilist) {
        m_map.insert(ilist);
    }
    
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return m_map.emplace(std::forward<Args>(args)...);
    }
    
    template <typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) {
        return m_map.emplace_hint(hint, std::forward<Args>(args)...);
    }
    
    iterator erase(const_iterator pos) {
        return m_map.erase(pos);
    }
    
    iterator erase(const_iterator first, const_iterator last) {
        return m_map.erase(first, last);
    }
    
    size_type erase(const key_type& key) {
        return m_map.erase(key);
    }
    
    void swap(map& other) noexcept(std::is_nothrow_swappable_v<Compare>) {
        m_map.swap(other.m_map);
    }
    
    // 查找
    size_type count(const Key& key) const {
        return m_map.count(key);
    }
    
    template <typename K>
    size_type count(const K& x) const {
        return m_map.count(x);
    }
    
    iterator find(const Key& key) {
        return m_map.find(key);
    }
    
    const_iterator find(const Key& key) const {
        return m_map.find(key);
    }
    
    template <typename K>
    iterator find(const K& x) {
        return m_map.find(x);
    }
    
    template <typename K>
    const_iterator find(const K& x) const {
        return m_map.find(x);
    }
    
    bool contains(const Key& key) const {
        return m_map.find(key) != m_map.end();
    }
    
    template <typename K>
    bool contains(const K& x) const {
        return m_map.find(x) != m_map.end();
    }
    
    std::pair<iterator, iterator> equal_range(const Key& key) {
        return m_map.equal_range(key);
    }
    
    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
        return m_map.equal_range(key);
    }
    
    template <typename K>
    std::pair<iterator, iterator> equal_range(const K& x) {
        return m_map.equal_range(x);
    }
    
    template <typename K>
    std::pair<const_iterator, const_iterator> equal_range(const K& x) const {
        return m_map.equal_range(x);
    }
    
    iterator lower_bound(const Key& key) {
        return m_map.lower_bound(key);
    }
    
    const_iterator lower_bound(const Key& key) const {
        return m_map.lower_bound(key);
    }
    
    template <typename K>
    iterator lower_bound(const K& x) {
        return m_map.lower_bound(x);
    }
    
    template <typename K>
    const_iterator lower_bound(const K& x) const {
        return m_map.lower_bound(x);
    }
    
    iterator upper_bound(const Key& key) {
        return m_map.upper_bound(key);
    }
    
    const_iterator upper_bound(const Key& key) const {
        return m_map.upper_bound(key);
    }
    
    template <typename K>
    iterator upper_bound(const K& x) {
        return m_map.upper_bound(x);
    }
    
    template <typename K>
    const_iterator upper_bound(const K& x) const {
        return m_map.upper_bound(x);
    }
    
    // 观察器
    key_compare key_comp() const {
        return m_map.key_comp();
    }
    
    value_compare value_comp() const {
        return value_compare(m_map.key_comp());
    }
    
    // 比较运算符
    friend bool operator==(const map& lhs, const map& rhs) {
        return lhs.m_map == rhs.m_map;
    }
    
    friend bool operator!=(const map& lhs, const map& rhs) {
        return lhs.m_map != rhs.m_map;
    }
    
    friend bool operator<(const map& lhs, const map& rhs) {
        return lhs.m_map < rhs.m_map;
    }
    
    friend bool operator<=(const map& lhs, const map& rhs) {
        return lhs.m_map <= rhs.m_map;
    }
    
    friend bool operator>(const map& lhs, const map& rhs) {
        return lhs.m_map > rhs.m_map;
    }
    
    friend bool operator>=(const map& lhs, const map& rhs) {
        return lhs.m_map >= rhs.m_map;
    }
};

// 特化的swap函数
template <typename Key, typename T, typename Compare, typename Alloc>
void swap(map<Key, T, Compare, Alloc>& lhs, map<Key, T, Compare, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace sjkxq_stl

#endif // SJKXQ_STL_MAP_HPP