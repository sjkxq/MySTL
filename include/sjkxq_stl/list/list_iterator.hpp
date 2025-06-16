#ifndef SJKXQ_STL_LIST_ITERATOR_HPP
#define SJKXQ_STL_LIST_ITERATOR_HPP

#include "../container_base/iterator_base.hpp"
#include "list_node.hpp"

namespace sjkxq_stl {

template <typename T>
class list_iterator : public bidirectional_iterator_base<T, list_node_impl<T>*> {
    using base = bidirectional_iterator_base<T, list_node_impl<T>*>;
    using node_type = list_node_impl<T>;

public:
    using iterator_category = typename base::iterator_category;
    using value_type = typename base::value_type;
    using difference_type = typename base::difference_type;
    using pointer = typename base::pointer;
    using reference = typename base::reference;

    explicit list_iterator(node_type* x = nullptr) noexcept 
        : base(x) {}

    reference operator*() const noexcept {
        return this->node->value;
    }

    pointer operator->() const noexcept {
        return std::addressof(this->node->value);
    }

    list_iterator& operator++() noexcept {
        base::operator++();
        return *this;
    }

    list_iterator operator++(int) noexcept {
        list_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    list_iterator& operator--() noexcept {
        base::operator--();
        return *this;
    }

    list_iterator operator--(int) noexcept {
        list_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const list_iterator& rhs) const noexcept {
        return base::operator==(rhs);
    }

    bool operator!=(const list_iterator& rhs) const noexcept {
        return base::operator!=(rhs);
    }
};

template <typename T>
class const_list_iterator : public bidirectional_iterator_base<const T, const list_node_impl<T>*> {
    using base = bidirectional_iterator_base<const T, const list_node_impl<T>*>;
    using node_type = const list_node_impl<T>;

public:
    using iterator_category = typename base::iterator_category;
    using value_type = typename base::value_type;
    using difference_type = typename base::difference_type;
    using pointer = typename base::pointer;
    using reference = typename base::reference;

    explicit const_list_iterator(node_type* x = nullptr) noexcept 
        : base(x) {}

    const_list_iterator(const list_iterator<T>& it) noexcept 
        : base(it.get_node()) {}

    reference operator*() const noexcept {
        return this->node->value;
    }

    pointer operator->() const noexcept {
        return std::addressof(this->node->value);
    }

    const_list_iterator& operator++() noexcept {
        base::operator++();
        return *this;
    }

    const_list_iterator operator++(int) noexcept {
        const_list_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    const_list_iterator& operator--() noexcept {
        base::operator--();
        return *this;
    }

    const_list_iterator operator--(int) noexcept {
        const_list_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    bool operator==(const const_list_iterator& rhs) const noexcept {
        return base::operator==(rhs);
    }

    bool operator!=(const const_list_iterator& rhs) const noexcept {
        return base::operator!=(rhs);
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_LIST_ITERATOR_HPP