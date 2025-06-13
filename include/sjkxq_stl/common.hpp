#ifndef SJKXQ_STL_COMMON_HPP
#define SJKXQ_STL_COMMON_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace sjkxq_stl {

// 类型别名
using size_type = std::size_t;

// 异常类
class out_of_range : public std::out_of_range {
public:
    explicit out_of_range(const std::string& what_arg)
        : std::out_of_range(what_arg) {}
    explicit out_of_range(const char* what_arg)
        : std::out_of_range(what_arg) {}
};

// 迭代器标签
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 迭代器特性
template <typename Iterator>
struct iterator_traits {
    using difference_type = typename Iterator::difference_type;
    using value_type = typename Iterator::value_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
    using iterator_category = typename Iterator::iterator_category;
};

// 针对指针的特化
template <typename T>
struct iterator_traits<T*> {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = random_access_iterator_tag;
};

template <typename T>
struct iterator_traits<const T*> {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using iterator_category = random_access_iterator_tag;
};

// 交换函数
template <typename T>
void swap(T& a, T& b) noexcept(std::is_nothrow_move_constructible<T>::value &&
                               std::is_nothrow_move_assignable<T>::value) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

} // namespace sjkxq_stl

#endif // SJKXQ_STL_COMMON_HPP