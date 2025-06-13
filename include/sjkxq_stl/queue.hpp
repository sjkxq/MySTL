#ifndef SJKXQ_STL_QUEUE_HPP
#define SJKXQ_STL_QUEUE_HPP

#include "common.hpp"
#include <deque>

namespace sjkxq_stl {

template <typename T, typename Container = std::deque<T>>
class queue {
public:
    // 类型定义
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

protected:
    Container c; // 底层容器

public:
    // 构造函数
    queue() : c() {}
    
    explicit queue(const Container& cont) : c(cont) {}
    
    explicit queue(Container&& cont) : c(std::move(cont)) {}
    
    queue(const queue& other) : c(other.c) {}
    
    queue(queue&& other) noexcept : c(std::move(other.c)) {}
    
    template <typename Alloc>
    explicit queue(const Alloc& alloc) : c(alloc) {}
    
    template <typename Alloc>
    queue(const Container& cont, const Alloc& alloc) : c(cont, alloc) {}
    
    template <typename Alloc>
    queue(Container&& cont, const Alloc& alloc) : c(std::move(cont), alloc) {}
    
    template <typename Alloc>
    queue(const queue& other, const Alloc& alloc) : c(other.c, alloc) {}
    
    template <typename Alloc>
    queue(queue&& other, const Alloc& alloc) : c(std::move(other.c), alloc) {}
    
    // 赋值运算符
    queue& operator=(const queue& other) {
        c = other.c;
        return *this;
    }
    
    queue& operator=(queue&& other) noexcept {
        c = std::move(other.c);
        return *this;
    }
    
    // 元素访问
    reference front() {
        return c.front();
    }
    
    const_reference front() const {
        return c.front();
    }
    
    reference back() {
        return c.back();
    }
    
    const_reference back() const {
        return c.back();
    }
    
    // 容量
    bool empty() const {
        return c.empty();
    }
    
    size_type size() const {
        return c.size();
    }
    
    // 修改器
    void push(const value_type& value) {
        c.push_back(value);
    }
    
    void push(value_type&& value) {
        c.push_back(std::move(value));
    }
    
    template <typename... Args>
    void emplace(Args&&... args) {
        c.emplace_back(std::forward<Args>(args)...);
    }
    
    void pop() {
        c.pop_front();
    }
    
    void swap(queue& other) noexcept(std::is_nothrow_swappable_v<Container>) {
        using std::swap;
        swap(c, other.c);
    }
    
    // 比较运算符
    friend bool operator==(const queue& lhs, const queue& rhs) {
        return lhs.c == rhs.c;
    }
    
    friend bool operator!=(const queue& lhs, const queue& rhs) {
        return lhs.c != rhs.c;
    }
    
    friend bool operator<(const queue& lhs, const queue& rhs) {
        return lhs.c < rhs.c;
    }
    
    friend bool operator<=(const queue& lhs, const queue& rhs) {
        return lhs.c <= rhs.c;
    }
    
    friend bool operator>(const queue& lhs, const queue& rhs) {
        return lhs.c > rhs.c;
    }
    
    friend bool operator>=(const queue& lhs, const queue& rhs) {
        return lhs.c >= rhs.c;
    }
};

// 特化的swap函数
template <typename T, typename Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace sjkxq_stl

#endif // SJKXQ_STL_QUEUE_HPP