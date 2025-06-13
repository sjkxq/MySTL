#ifndef SJKXQ_STL_STACK_HPP
#define SJKXQ_STL_STACK_HPP

#include "common.hpp"
#include <deque>

namespace sjkxq_stl {

template <typename T, typename Container = std::deque<T>>
class stack {
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
    stack() : c() {}
    
    explicit stack(const Container& cont) : c(cont) {}
    
    explicit stack(Container&& cont) : c(std::move(cont)) {}
    
    stack(const stack& other) : c(other.c) {}
    
    stack(stack&& other) noexcept : c(std::move(other.c)) {}
    
    template <typename Alloc>
    explicit stack(const Alloc& alloc) : c(alloc) {}
    
    template <typename Alloc>
    stack(const Container& cont, const Alloc& alloc) : c(cont, alloc) {}
    
    template <typename Alloc>
    stack(Container&& cont, const Alloc& alloc) : c(std::move(cont), alloc) {}
    
    template <typename Alloc>
    stack(const stack& other, const Alloc& alloc) : c(other.c, alloc) {}
    
    template <typename Alloc>
    stack(stack&& other, const Alloc& alloc) : c(std::move(other.c), alloc) {}
    
    // 赋值运算符
    stack& operator=(const stack& other) {
        c = other.c;
        return *this;
    }
    
    stack& operator=(stack&& other) noexcept {
        c = std::move(other.c);
        return *this;
    }
    
    // 元素访问
    reference top() {
        return c.back();
    }
    
    const_reference top() const {
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
        c.pop_back();
    }
    
    void swap(stack& other) noexcept(std::is_nothrow_swappable_v<Container>) {
        using std::swap;
        swap(c, other.c);
    }
    
    // 比较运算符
    friend bool operator==(const stack& lhs, const stack& rhs) {
        return lhs.c == rhs.c;
    }
    
    friend bool operator!=(const stack& lhs, const stack& rhs) {
        return lhs.c != rhs.c;
    }
    
    friend bool operator<(const stack& lhs, const stack& rhs) {
        return lhs.c < rhs.c;
    }
    
    friend bool operator<=(const stack& lhs, const stack& rhs) {
        return lhs.c <= rhs.c;
    }
    
    friend bool operator>(const stack& lhs, const stack& rhs) {
        return lhs.c > rhs.c;
    }
    
    friend bool operator>=(const stack& lhs, const stack& rhs) {
        return lhs.c >= rhs.c;
    }
};

// 特化的swap函数
template <typename T, typename Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace sjkxq_stl

#endif // SJKXQ_STL_STACK_HPP