#ifndef SJKXQ_STL_LIST_BASE_HPP
#define SJKXQ_STL_LIST_BASE_HPP

#include "../container_base/memory_base.hpp"
#include "../container_base/node_base.hpp"
#include "list_node.hpp"
#include "list_iterator.hpp"
#include <memory>
#include <stdexcept>

namespace sjkxq_stl {

template <typename T, typename Allocator = std::allocator<T>>
class list_base : protected memory_base<T, Allocator> {
public:
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

protected:
    // 检查索引是否越界
    void range_check(size_type pos, size_type size) const {
        if (pos >= size)
            throw std::out_of_range("container index out of range");
    }

    // 检查迭代器是否属于同一个容器
    template <typename Iterator1, typename Iterator2>
    static bool iterator_in_same_container(const Iterator1& it1, const Iterator2& it2) {
        return it1.node->next->prev == it2.node->prev->next;
    }
protected:
    using memory_base_type = memory_base<T, Allocator>;
    using node_type = list_node_impl<T>;
    using node_allocator_type = list_node_allocator<T, Allocator>;

    node_base header;
    node_allocator_type node_alloc;
    size_t node_count;

    list_base(const Allocator& a = Allocator())
        : memory_base_type(a), node_alloc(a), node_count(0) {}

    node_type* get_node() {
        return node_alloc.allocate();
    }

    void put_node(node_type* p) {
        node_alloc.deallocate(p);
    }

    template <typename... Args>
    node_type* create_node(Args&&... args) {
        node_type* p = get_node();
        try {
            node_alloc.construct(p, std::forward<Args>(args)...);
            return p;
        }
        catch (...) {
            put_node(p);
            throw;
        }
    }

    void destroy_node(node_type* p) {
        node_alloc.destroy(p);
        put_node(p);
    }

    void init() noexcept {
        header.next = &header;
        header.prev = &header;
        node_count = 0;
    }

public:
    using iterator = list_iterator<T>;
    using const_iterator = const_list_iterator<T>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Allocator;

protected:
    node_type* begin_node() noexcept {
        return static_cast<node_type*>(header.next);
    }

    const node_type* begin_node() const noexcept {
        return static_cast<const node_type*>(header.next);
    }

    node_type* end_node() noexcept {
        return static_cast<node_type*>(&header);
    }

    const node_type* end_node() const noexcept {
        return static_cast<const node_type*>(&header);
    }

    iterator make_iterator(node_type* node) noexcept {
        return iterator(node);
    }

    const_iterator make_const_iterator(const node_type* node) const noexcept {
        return const_iterator(node);
    }

    void increase_size(size_type n) noexcept {
        node_count += n;
    }

    void decrease_size(size_type n) noexcept {
        node_count -= n;
    }

    size_type get_size() const noexcept {
        return node_count;
    }

    bool empty() const noexcept {
        return node_count == 0;
    }

    void transfer(iterator position, iterator first, iterator last) {
        position.get_node()->transfer(first.get_node(), last.get_node());
    }

    void swap_data(list_base& other) noexcept {
        std::swap(header.next, other.header.next);
        std::swap(header.prev, other.header.prev);
        std::swap(node_count, other.node_count);
        
        if (header.next == &other.header)
            header.next = header.prev = &header;
        else {
            header.next->prev = &header;
            header.prev->next = &header;
        }
        
        if (other.header.next == &header)
            other.header.next = other.header.prev = &other.header;
        else {
            other.header.next->prev = &other.header;
            other.header.prev->next = &other.header;
        }
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_LIST_BASE_HPP