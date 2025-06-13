#ifndef SJKXQ_STL_LIST_HPP
#define SJKXQ_STL_LIST_HPP

#include "common.hpp"
#include <memory>
#include <initializer_list>

namespace sjkxq_stl {

template <typename T, typename Allocator = std::allocator<T>>
class list {
private:
    // 链表节点结构
    struct node {
        node* prev;
        node* next;
        T data;
        
        node(const T& value) : prev(nullptr), next(nullptr), data(value) {}
        node(T&& value) : prev(nullptr), next(nullptr), data(std::move(value)) {}
    };
    
    // 使用rebind获取节点分配器类型
    using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
    
public:
    // 迭代器类
    class iterator {
    private:
        node* ptr_;
        
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = bidirectional_iterator_tag;
        
        iterator() noexcept : ptr_(nullptr) {}
        explicit iterator(node* ptr) noexcept : ptr_(ptr) {}
        
        reference operator*() const {
            return ptr_->data;
        }
        
        pointer operator->() const {
            return &(ptr_->data);
        }
        
        iterator& operator++() {
            ptr_ = ptr_->next;
            return *this;
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        iterator& operator--() {
            ptr_ = ptr_->prev;
            return *this;
        }
        
        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        bool operator==(const iterator& other) const {
            return ptr_ == other.ptr_;
        }
        
        bool operator!=(const iterator& other) const {
            return ptr_ != other.ptr_;
        }
        
        // 友元声明，允许list访问迭代器的私有成员
        friend class list;
    };
    
    // 常量迭代器类
    class const_iterator {
    private:
        const node* ptr_;
        
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = bidirectional_iterator_tag;
        
        const_iterator() noexcept : ptr_(nullptr) {}
        explicit const_iterator(const node* ptr) noexcept : ptr_(ptr) {}
        const_iterator(const iterator& it) noexcept : ptr_(it.ptr_) {}
        
        reference operator*() const {
            return ptr_->data;
        }
        
        pointer operator->() const {
            return &(ptr_->data);
        }
        
        const_iterator& operator++() {
            ptr_ = ptr_->next;
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        const_iterator& operator--() {
            ptr_ = ptr_->prev;
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }
        
        bool operator==(const const_iterator& other) const {
            return ptr_ == other.ptr_;
        }
        
        bool operator!=(const const_iterator& other) const {
            return ptr_ != other.ptr_;
        }
        
        // 友元声明，允许list访问迭代器的私有成员
        friend class list;
    };
    
    // 类型定义
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = sjkxq_stl::size_type;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    
private:
    node* head_;        // 头节点（哨兵节点，不存储实际数据）
    size_type size_;    // 链表大小
    node_allocator alloc_; // 节点分配器
    
    // 创建一个新节点
    template <typename... Args>
    node* create_node(Args&&... args) {
        node* new_node = alloc_.allocate(1);
        try {
            std::allocator_traits<node_allocator>::construct(alloc_, new_node, std::forward<Args>(args)...);
            return new_node;
        } catch (...) {
            alloc_.deallocate(new_node, 1);
            throw;
        }
    }
    
    // 销毁一个节点
    void destroy_node(node* n) {
        std::allocator_traits<node_allocator>::destroy(alloc_, n);
        alloc_.deallocate(n, 1);
    }
    
    // 初始化空链表
    void init() {
        head_ = alloc_.allocate(1);
        std::allocator_traits<node_allocator>::construct(alloc_, head_, T());
        head_->next = head_;
        head_->prev = head_;
        size_ = 0;
    }
    
public:
    // 构造函数
    list() : size_(0), alloc_() {
        init();
    }
    
    explicit list(const Allocator& alloc) : size_(0), alloc_(alloc) {
        init();
    }
    
    list(size_type count, const T& value, const Allocator& alloc = Allocator())
        : size_(0), alloc_(alloc) {
        init();
        for (size_type i = 0; i < count; ++i) {
            push_back(value);
        }
    }
    
    explicit list(size_type count, const Allocator& alloc = Allocator())
        : size_(0), alloc_(alloc) {
        init();
        for (size_type i = 0; i < count; ++i) {
            emplace_back();
        }
    }
    
    list(std::initializer_list<T> ilist, const Allocator& alloc = Allocator())
        : size_(0), alloc_(alloc) {
        init();  // 初始化头节点
        for (const auto& value : ilist) {
            push_back(value);
        }
    }
    
    // 复制构造函数
    list(const list& other)
        : size_(0), alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_)) {
        init();
        for (const auto& value : other) {
            push_back(value);
        }
    }
    
    // 移动构造函数
    list(list&& other) noexcept
        : head_(other.head_), size_(other.size_), alloc_(std::move(other.alloc_)) {
        other.init();
    }
    
    // 析构函数
    ~list() {
        clear();
        destroy_node(head_);
    }
    
    // 赋值运算符
    list& operator=(const list& other) {
        if (this != &other) {
            clear();
            
            if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
                alloc_ = other.alloc_;
            }
            
            for (const auto& value : other) {
                push_back(value);
            }
        }
        return *this;
    }
    
    // 移动赋值运算符
    list& operator=(list&& other) noexcept {
        if (this != &other) {
            clear();
            destroy_node(head_);
            
            if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
                alloc_ = std::move(other.alloc_);
            }
            
            head_ = other.head_;
            size_ = other.size_;
            
            other.init();
        }
        return *this;
    }
    
    // 元素访问
    reference front() {
        return head_->next->data;
    }
    
    const_reference front() const {
        return head_->next->data;
    }
    
    reference back() {
        return head_->prev->data;
    }
    
    const_reference back() const {
        return head_->prev->data;
    }
    
    // 迭代器
    iterator begin() noexcept {
        return iterator(head_->next);
    }
    
    const_iterator begin() const noexcept {
        return const_iterator(head_->next);
    }
    
    const_iterator cbegin() const noexcept {
        return const_iterator(head_->next);
    }
    
    iterator end() noexcept {
        return iterator(head_);
    }
    
    const_iterator end() const noexcept {
        return const_iterator(head_);
    }
    
    const_iterator cend() const noexcept {
        return const_iterator(head_);
    }
    
    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }
    
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }
    
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }
    
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }
    
    // 容量
    bool empty() const noexcept {
        return size_ == 0;
    }
    
    size_type size() const noexcept {
        return size_;
    }
    
    size_type max_size() const noexcept {
        return std::allocator_traits<node_allocator>::max_size(alloc_);
    }
    
    // 修改器
    void clear() noexcept {
        node* current = head_->next;
        while (current != head_) {
            node* next = current->next;
            destroy_node(current);
            current = next;
        }
        head_->next = head_;
        head_->prev = head_;
        size_ = 0;
    }
    
    iterator insert(const_iterator pos, const T& value) {
        node* new_node = create_node(value);
        node* pos_node = const_cast<node*>(pos.ptr_);
        
        new_node->next = pos_node;
        new_node->prev = pos_node->prev;
        pos_node->prev->next = new_node;
        pos_node->prev = new_node;
        
        ++size_;
        return iterator(new_node);
    }
    
    iterator insert(const_iterator pos, T&& value) {
        node* new_node = create_node(std::move(value));
        node* pos_node = const_cast<node*>(pos.ptr_);
        
        new_node->next = pos_node;
        new_node->prev = pos_node->prev;
        pos_node->prev->next = new_node;
        pos_node->prev = new_node;
        
        ++size_;
        return iterator(new_node);
    }
    
    iterator erase(const_iterator pos) {
        node* pos_node = const_cast<node*>(pos.ptr_);
        node* next_node = pos_node->next;
        
        pos_node->prev->next = next_node;
        next_node->prev = pos_node->prev;
        
        destroy_node(pos_node);
        --size_;
        
        return iterator(next_node);
    }
    
    void push_back(const T& value) {
        insert(end(), value);
    }
    
    void push_back(T&& value) {
        insert(end(), std::move(value));
    }
    
    void push_front(const T& value) {
        insert(begin(), value);
    }
    
    void push_front(T&& value) {
        insert(begin(), std::move(value));
    }
    
    void pop_back() {
        erase(--end());
    }
    
    void pop_front() {
        erase(begin());
    }
    
    template <typename... Args>
    reference emplace_back(Args&&... args) {
        node* new_node = create_node(T(std::forward<Args>(args)...));
        
        new_node->next = head_;
        new_node->prev = head_->prev;
        head_->prev->next = new_node;
        head_->prev = new_node;
        
        ++size_;
        return new_node->data;
    }
    
    template <typename... Args>
    reference emplace_front(Args&&... args) {
        node* new_node = create_node(T(std::forward<Args>(args)...));
        
        new_node->next = head_->next;
        new_node->prev = head_;
        head_->next->prev = new_node;
        head_->next = new_node;
        
        ++size_;
        return new_node->data;
    }
    
    // 比较运算符
    friend bool operator==(const list& lhs, const list& rhs) {
        if (lhs.size_ != rhs.size_) {
            return false;
        }
        
        auto it1 = lhs.begin();
        auto it2 = rhs.begin();
        
        while (it1 != lhs.end()) {
            if (!(*it1 == *it2)) {
                return false;
            }
            ++it1;
            ++it2;
        }
        
        return true;
    }
    
    friend bool operator!=(const list& lhs, const list& rhs) {
        return !(lhs == rhs);
    }
    
    friend bool operator<(const list& lhs, const list& rhs) {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    
    friend bool operator<=(const list& lhs, const list& rhs) {
        return !(rhs < lhs);
    }
    
    friend bool operator>(const list& lhs, const list& rhs) {
        return rhs < lhs;
    }
    
    friend bool operator>=(const list& lhs, const list& rhs) {
        return !(lhs < rhs);
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_LIST_HPP