#ifndef SJKXQ_STL_ITERATOR_BASE_HPP
#define SJKXQ_STL_ITERATOR_BASE_HPP

#include "../common.hpp"
#include <iterator>

namespace sjkxq_stl {

// 基础迭代器类，提供通用功能
template <typename Category,
          typename T,
          typename Distance = std::ptrdiff_t,
          typename Pointer = T*,
          typename Reference = T&>
struct iterator_base {
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using reference = Reference;
};

// 双向链表迭代器基类
template <typename T, typename NodePtr>
class bidirectional_iterator_base 
    : public iterator_base<bidirectional_iterator_tag, T> {
protected:
    NodePtr node;

    explicit bidirectional_iterator_base(NodePtr x) noexcept : node(x) {}

public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    NodePtr get_node() const noexcept { return node; }

    bidirectional_iterator_base& operator++() noexcept {
        node = static_cast<NodePtr>(node->next);
        return *this;
    }

    bidirectional_iterator_base operator++(int) noexcept {
        bidirectional_iterator_base tmp = *this;
        ++(*this);
        return tmp;
    }

    bidirectional_iterator_base& operator--() noexcept {
        node = static_cast<NodePtr>(node->prev);
        return *this;
    }

    bidirectional_iterator_base operator--(int) noexcept {
        bidirectional_iterator_base tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const bidirectional_iterator_base& rhs) const noexcept {
        return node == rhs.node;
    }

    bool operator!=(const bidirectional_iterator_base& rhs) const noexcept {
        return !(*this == rhs);
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_ITERATOR_BASE_HPP