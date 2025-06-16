#ifndef SJKXQ_STL_LIST_HPP
#define SJKXQ_STL_LIST_HPP

#include "list_base.hpp"
#include <algorithm>
#include <initializer_list>

namespace sjkxq_stl {

template <typename T, typename Allocator>
class list : public list_base<T, Allocator> {
    using base = list_base<T, Allocator>;

public:
    using value_type = typename base::value_type;
    using allocator_type = typename base::allocator_type;
    using size_type = typename base::size_type;
    using difference_type = typename base::difference_type;
    using reference = typename base::reference;
    using const_reference = typename base::const_reference;
    using pointer = typename base::pointer;
    using const_pointer = typename base::const_pointer;
    using iterator = typename base::iterator;
    using const_iterator = typename base::const_iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // 构造函数
    list() : base() {
        this->init();
    }

    explicit list(const allocator_type& alloc) : base(alloc) {
        this->init();
    }

    explicit list(size_type n, const allocator_type& alloc = allocator_type())
        : base(alloc) {
        this->init();
        insert(begin(), n, value_type());
    }

    list(size_type n, const value_type& value, const allocator_type& alloc = allocator_type())
        : base(alloc) {
        this->init();
        insert(begin(), n, value);
    }

    template <typename InputIt>
    list(InputIt first, InputIt last, const allocator_type& alloc = allocator_type())
        : base(alloc) {
        this->init();
        insert(begin(), first, last);
    }

    list(const list& other)
        : base(other.get_allocator()) {
        this->init();
        insert(begin(), other.begin(), other.end());
    }

    list(list&& other) noexcept
        : base(std::move(other.get_allocator())) {
        this->init();
        this->swap_data(other);
    }

    list(std::initializer_list<value_type> init, const allocator_type& alloc = allocator_type())
        : base(alloc) {
        this->init();
        insert(begin(), init.begin(), init.end());
    }

    ~list() {
        clear();
    }

    // 赋值操作符
    list& operator=(const list& other) {
        if (this != &other) {
            clear();
            insert(begin(), other.begin(), other.end());
        }
        return *this;
    }

    list& operator=(list&& other) noexcept {
        if (this != &other) {
            clear();
            this->swap_data(other);
        }
        return *this;
    }

    list& operator=(std::initializer_list<value_type> init) {
        clear();
        insert(begin(), init.begin(), init.end());
        return *this;
    }

    // 迭代器
    iterator begin() noexcept {
        return this->make_iterator(this->begin_node());
    }

    const_iterator begin() const noexcept {
        return this->make_const_iterator(this->begin_node());
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    iterator end() noexcept {
        return this->make_iterator(this->end_node());
    }

    const_iterator end() const noexcept {
        return this->make_const_iterator(this->end_node());
    }

    const_iterator cend() const noexcept {
        return end();
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept {
        return rbegin();
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept {
        return rend();
    }

    // 容量
    bool empty() const noexcept {
        return this->base::empty();
    }

    size_type size() const noexcept {
        return this->get_size();
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    // 元素访问
    reference front() {
        return *begin();
    }

    const_reference front() const {
        return *begin();
    }

    reference back() {
        iterator tmp = end();
        --tmp;
        return *tmp;
    }

    const_reference back() const {
        const_iterator tmp = end();
        --tmp;
        return *tmp;
    }

    // 修改器
    void clear() noexcept {
        erase(begin(), end());
    }

    iterator insert(const_iterator pos, const value_type& value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, value_type&& value) {
        return emplace(pos, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const value_type& value) {
        if (count == 0) {
            return iterator(const_cast<typename base::node_type*>(pos.get_node()));
        }

        iterator result(const_cast<typename base::node_type*>(pos.get_node()));
        for (size_type i = 0; i < count; ++i) {
            result = insert(pos, value);
        }
        return result;
    }

    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        if (first == last) {
            return iterator(const_cast<typename base::node_type*>(pos.get_node()));
        }

        iterator result(const_cast<typename base::node_type*>(pos.get_node()));
        for (; first != last; ++first) {
            result = insert(pos, *first);
        }
        return result;
    }

    iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        typename base::node_type* new_node = this->create_node(std::forward<Args>(args)...);
        new_node->hook(const_cast<node_base*>(static_cast<const node_base*>(pos.get_node())));
        this->increase_size(1);
        return iterator(new_node);
    }

    iterator erase(const_iterator pos) {
        typename base::node_type* node = const_cast<typename base::node_type*>(pos.get_node());
        iterator result(static_cast<typename base::node_type*>(node->next));
        node->unhook();
        this->destroy_node(node);
        this->decrease_size(1);
        return result;
    }

    iterator erase(const_iterator first, const_iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return iterator(const_cast<typename base::node_type*>(last.get_node()));
    }

    void push_back(const value_type& value) {
        insert(end(), value);
    }

    void push_back(value_type&& value) {
        insert(end(), std::move(value));
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        emplace(end(), std::forward<Args>(args)...);
        return back();
    }

    void pop_back() {
        erase(--end());
    }

    void push_front(const value_type& value) {
        insert(begin(), value);
    }

    void push_front(value_type&& value) {
        insert(begin(), std::move(value));
    }

    template <typename... Args>
    reference emplace_front(Args&&... args) {
        emplace(begin(), std::forward<Args>(args)...);
        return front();
    }

    void pop_front() {
        erase(begin());
    }

    void resize(size_type count) {
        resize(count, value_type());
    }

    void resize(size_type count, const value_type& value) {
        if (count > size()) {
            insert(end(), count - size(), value);
        } else if (count < size()) {
            iterator it = begin();
            std::advance(it, count);
            erase(it, end());
        }
    }

    void swap(list& other) noexcept {
        if (this != &other) {
            this->swap_data(other);
        }
    }

    // 操作
    void merge(list& other) {
        merge(std::move(other), std::less<value_type>());
    }

    void merge(list&& other) {
        merge(std::move(other), std::less<value_type>());
    }

    template <typename Compare>
    void merge(list& other, Compare comp) {
        merge(std::move(other), comp);
    }

    template <typename Compare>
    void merge(list&& other, Compare comp) {
        if (this == &other) {
            return;
        }

        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = other.begin();
        iterator last2 = other.end();

        while (first1 != last1 && first2 != last2) {
            if (comp(*first2, *first1)) {
                iterator next = first2;
                ++next;
                this->transfer(first1, first2, next);
                first2 = next;
            } else {
                ++first1;
            }
        }

        if (first2 != last2) {
            this->transfer(last1, first2, last2);
        }

        this->increase_size(other.size());
        other.node_count = 0;
    }

    void splice(const_iterator pos, list& other) {
        splice(pos, std::move(other));
    }

    void splice(const_iterator pos, list&& other) {
        if (!other.empty()) {
            this->transfer(iterator(const_cast<typename base::node_type*>(pos.get_node())),
                          other.begin(), other.end());
            this->increase_size(other.size());
            other.node_count = 0;
        }
    }

    void splice(const_iterator pos, list& other, const_iterator it) {
        splice(pos, std::move(other), it);
    }

    void splice(const_iterator pos, list&& other, const_iterator it) {
        iterator inext(const_cast<typename base::node_type*>(it.get_node()));
        ++inext;
        if (pos == it || pos == inext) {
            return;
        }

        this->transfer(iterator(const_cast<typename base::node_type*>(pos.get_node())),
                      iterator(const_cast<typename base::node_type*>(it.get_node())),
                      inext);
        this->increase_size(1);
        other.decrease_size(1);
    }

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) {
        splice(pos, std::move(other), first, last);
    }

    void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last) {
        if (first != last) {
            size_type n = std::distance(first, last);
            this->transfer(iterator(const_cast<typename base::node_type*>(pos.get_node())),
                          iterator(const_cast<typename base::node_type*>(first.get_node())),
                          iterator(const_cast<typename base::node_type*>(last.get_node())));
            this->increase_size(n);
            other.decrease_size(n);
        }
    }

    void remove(const value_type& value) {
        remove_if([&value](const value_type& x) { return x == value; });
    }

    template <typename UnaryPredicate>
    void remove_if(UnaryPredicate p) {
        for (iterator it = begin(); it != end();) {
            if (p(*it)) {
                it = erase(it);
            } else {
                ++it;
            }
        }
    }

    void reverse() noexcept {
        if (size() <= 1) {
            return;
        }

        iterator it = begin();
        ++it;
        while (it != end()) {
            iterator old = it;
            ++it;
            this->transfer(begin(), old, it);
        }
    }

    void unique() {
        unique(std::equal_to<value_type>());
    }

    template <typename BinaryPredicate>
    void unique(BinaryPredicate p) {
        if (size() <= 1) {
            return;
        }

        iterator first = begin();
        iterator last = end();
        iterator next = first;
        ++next;

        while (next != last) {
            if (p(*first, *next)) {
                next = erase(next);
            } else {
                first = next;
                ++next;
            }
        }
    }

    void sort() {
        sort(std::less<value_type>());
    }

    template <typename Compare>
    void sort(Compare comp) {
        if (size() <= 1) {
            return;
        }

        list carry;
        list tmp[64];
        list* fill = &tmp[0];
        list* counter = nullptr;

        do {
            carry.splice(carry.begin(), *this, begin());

            for (counter = &tmp[0]; counter != fill && !counter->empty(); ++counter) {
                counter->merge(carry, comp);
                carry.swap(*counter);
            }

            carry.swap(*counter);
            if (counter == fill) {
                ++fill;
            }
        } while (!empty());

        for (counter = &tmp[1]; counter != fill; ++counter) {
            counter->merge(*(counter - 1), comp);
        }

        swap(*(fill - 1));
    }

    // 获取分配器
    allocator_type get_allocator() const noexcept {
        return this->memory_base_type::get_allocator();
    }
};

// 非成员函数
template <typename T, typename Alloc>
bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Alloc>
bool operator!=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <typename T, typename Alloc>
bool operator<(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Alloc>
bool operator<=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <typename T, typename Alloc>
bool operator>(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return rhs < lhs;
}

template <typename T, typename Alloc>
bool operator>=(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
    return !(lhs < rhs);
}

template <typename T, typename Alloc>
void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace sjkxq_stl

#endif // SJKXQ_STL_LIST_HPP