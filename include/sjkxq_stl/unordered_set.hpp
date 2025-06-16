#ifndef SJKXQ_STL_UNORDERED_SET_HPP
#define SJKXQ_STL_UNORDERED_SET_HPP

#include <functional>  // for std::hash, std::equal_to
#include <memory>     // for std::allocator
#include <utility>    // for std::pair
#include <cstddef>    // for size_t
#include <iterator>   // for iterator tags
#include <limits>     // for std::numeric_limits
#include <cmath>      // for std::ceil

namespace sjkxq_stl {

template <
    typename Key,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>
>
class unordered_set {
private:
    // 节点结构
    struct Node {
        Key value;
        Node* next;
        
        // 构造函数
        explicit Node(const Key& val) : value(val), next(nullptr) {}
        explicit Node(Key&& val) : value(std::move(val)), next(nullptr) {}
    };

public:
    // 节点句柄类型
    class node_type {
    private:
        friend class unordered_set;
        Node* node_;
        
        explicit node_type(Node* node = nullptr) noexcept : node_(node) {}
        
    public:
        node_type() noexcept : node_(nullptr) {}
        node_type(node_type&& other) noexcept : node_(other.node_) {
            other.node_ = nullptr;
        }
        
        ~node_type() {
            if (node_) {
                delete node_;
            }
        }
        
        node_type& operator=(node_type&& other) noexcept {
            if (this != &other) {
                if (node_) {
                    delete node_;
                }
                node_ = other.node_;
                other.node_ = nullptr;
            }
            return *this;
        }
        
        bool empty() const noexcept { return !node_; }
        explicit operator bool() const noexcept { return !empty(); }
        
        Key& value() const {
            if (!node_) {
                throw std::runtime_error("Accessing empty node");
            }
            return node_->value;
        }
        
        // 禁用复制操作
        node_type(const node_type&) = delete;
        node_type& operator=(const node_type&) = delete;
    };

    // 类型定义
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    // 迭代器类定义
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        iterator() noexcept : node_(nullptr), container_(nullptr), bucket_idx_(0) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &(node_->value); }

        iterator& operator++() {
            if (!node_) {
                return *this;  // 如果已经是end()迭代器，直接返回
            }
            
            if (node_->next) {
                // 如果当前节点有下一个节点，移动到下一个节点
                node_ = node_->next;
            } else {
                // 否则，查找下一个非空桶
                ++bucket_idx_;
                while (bucket_idx_ < container_->bucket_count_ && !container_->buckets_[bucket_idx_]) {
                    ++bucket_idx_;
                }

                if (bucket_idx_ < container_->bucket_count_) {
                    node_ = container_->buckets_[bucket_idx_];
                } else {
                    node_ = nullptr; // 到达末尾
                }
            }
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

    private:
        friend class unordered_set;
        friend class const_iterator;

        Node* node_;
        const unordered_set* container_;
        size_type bucket_idx_;

        iterator(Node* node, const unordered_set* container, size_type bucket_idx)
            : node_(node), container_(container), bucket_idx_(bucket_idx) {}
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_iterator() noexcept : node_(nullptr), container_(nullptr), bucket_idx_(0) {}
        const_iterator(const iterator& it) noexcept : node_(it.node_), container_(it.container_), bucket_idx_(it.bucket_idx_) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &(node_->value); }

        const_iterator& operator++() {
            if (!node_) {
                return *this;  // 如果已经是end()迭代器，直接返回
            }
            
            if (node_->next) {
                // 如果当前节点有下一个节点，移动到下一个节点
                node_ = node_->next;
            } else {
                // 否则，查找下一个非空桶
                ++bucket_idx_;
                while (bucket_idx_ < container_->bucket_count_ && !container_->buckets_[bucket_idx_]) {
                    ++bucket_idx_;
                }

                if (bucket_idx_ < container_->bucket_count_) {
                    node_ = container_->buckets_[bucket_idx_];
                } else {
                    node_ = nullptr; // 到达末尾
                }
            }
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

    private:
        friend class unordered_set;

        const Node* node_;
        const unordered_set* container_;
        size_type bucket_idx_;

        const_iterator(const Node* node, const unordered_set* container, size_type bucket_idx)
            : node_(node), container_(container), bucket_idx_(bucket_idx) {}
    };

    class local_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        local_iterator() noexcept : node_(nullptr) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &(node_->value); }

        local_iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        local_iterator operator++(int) {
            local_iterator tmp = *this;
            node_ = node_->next;
            return tmp;
        }

        bool operator==(const local_iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const local_iterator& other) const {
            return !(*this == other);
        }

    private:
        friend class unordered_set;
        friend class const_local_iterator;

        Node* node_;

        explicit local_iterator(Node* node) : node_(node) {}
    };

    class const_local_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Key;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        const_local_iterator() noexcept : node_(nullptr) {}
        const_local_iterator(const local_iterator& it) noexcept : node_(it.node_) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &(node_->value); }

        const_local_iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        const_local_iterator operator++(int) {
            const_local_iterator tmp = *this;
            node_ = node_->next;
            return tmp;
        }

        bool operator==(const const_local_iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const const_local_iterator& other) const {
            return !(*this == other);
        }

    private:
        friend class unordered_set;

        const Node* node_;

        explicit const_local_iterator(const Node* node) : node_(node) {}
    };
private:
    // 基本成员变量
    Node** buckets_;          // 桶数组
    size_type bucket_count_;  // 桶数量
    size_type size_;         // 元素数量
    float max_load_factor_;  // 最大负载因子
    hasher hash_function_;   // 哈希函数对象
    key_equal key_equal_;    // 键比较函数对象

    // 辅助函数
    size_type hash_to_bucket(const key_type& key) const {
        return hash_function_(key) % bucket_count_;
    }

    // 查找节点
    Node* find_node(const key_type& key) const {
        if (bucket_count_ == 0) return nullptr;
        
        size_type bucket_idx = hash_to_bucket(key);
        Node* current = buckets_[bucket_idx];
        
        while (current) {
            if (key_equal_(current->value, key)) {
                return current;
            }
            current = current->next;
        }
        
        return nullptr;
    }

public:
    // 构造函数
    unordered_set() 
        : buckets_(nullptr)
        , bucket_count_(0)
        , size_(0)
        , max_load_factor_(1.0f)
        , hash_function_()
        , key_equal_() {
        rehash(16);  // 默认16个桶
    }

    explicit unordered_set(size_type bucket_count)
        : buckets_(nullptr)
        , bucket_count_(0)
        , size_(0)
        , max_load_factor_(1.0f)
        , hash_function_()
        , key_equal_() {
        rehash(bucket_count);
    }

    unordered_set(std::initializer_list<value_type> init,
                 size_type bucket_count = 16)
        : unordered_set(bucket_count) {
        for (const auto& value : init) {
            insert(value);
        }
    }

    unordered_set(const unordered_set& other)
        : buckets_(nullptr)
        , bucket_count_(0)
        , size_(0)
        , max_load_factor_(other.max_load_factor_)
        , hash_function_(other.hash_function_)
        , key_equal_(other.key_equal_) {
        rehash(other.bucket_count_);
        for (const auto& value : other) {
            insert(value);
        }
    }

    unordered_set(unordered_set&& other) noexcept
        : buckets_(other.buckets_)
        , bucket_count_(other.bucket_count_)
        , size_(other.size_)
        , max_load_factor_(other.max_load_factor_)
        , hash_function_(std::move(other.hash_function_))
        , key_equal_(std::move(other.key_equal_)) {
        other.buckets_ = nullptr;
        other.bucket_count_ = 0;
        other.size_ = 0;
    }

    unordered_set& operator=(const unordered_set& other) {
        if (this != &other) {
            clear();
            max_load_factor_ = other.max_load_factor_;
            hash_function_ = other.hash_function_;
            key_equal_ = other.key_equal_;
            rehash(other.bucket_count_);
            for (const auto& value : other) {
                insert(value);
            }
        }
        return *this;
    }

    unordered_set& operator=(unordered_set&& other) noexcept {
        if (this != &other) {
            clear();
            delete[] buckets_;
            
            buckets_ = other.buckets_;
            bucket_count_ = other.bucket_count_;
            size_ = other.size_;
            max_load_factor_ = other.max_load_factor_;
            hash_function_ = std::move(other.hash_function_);
            key_equal_ = std::move(other.key_equal_);
            
            other.buckets_ = nullptr;
            other.bucket_count_ = 0;
            other.size_ = 0;
        }
        return *this;
    }

    unordered_set& operator=(std::initializer_list<value_type> ilist) {
        clear();
        for (const auto& value : ilist) {
            insert(value);
        }
        return *this;
    }

    // 析构函数
    ~unordered_set() {
        clear();
        delete[] buckets_;
    }

    // 基本容量操作
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type bucket_count() const noexcept { return bucket_count_; }
    
    // 哈希策略
    float load_factor() const noexcept {
        return bucket_count_ ? static_cast<float>(size_) / bucket_count_ : 0.0f;
    }
    
    float max_load_factor() const noexcept {
        return max_load_factor_;
    }
    
    void max_load_factor(float ml) {
        max_load_factor_ = ml;
    }

    // 迭代器操作
    iterator begin() noexcept {
        if (size_ == 0) return end();
        
        // 找到第一个非空桶
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (buckets_[i]) {
                return iterator(buckets_[i], this, i);
            }
        }
        return end();
    }

    const_iterator begin() const noexcept {
        return cbegin();
    }

    const_iterator cbegin() const noexcept {
        if (size_ == 0) return cend();
        
        // 找到第一个非空桶
        for (size_type i = 0; i < bucket_count_; ++i) {
            if (buckets_[i]) {
                return const_iterator(buckets_[i], this, i);
            }
        }
        return cend();
    }

    iterator end() noexcept {
        return iterator(nullptr, this, bucket_count_);
    }

    const_iterator end() const noexcept {
        return cend();
    }

    const_iterator cend() const noexcept {
        return const_iterator(nullptr, this, bucket_count_);
    }

    // 桶迭代器
    local_iterator begin(size_type n) {
        return local_iterator(buckets_[n]);
    }

    const_local_iterator begin(size_type n) const {
        return const_local_iterator(buckets_[n]);
    }

    const_local_iterator cbegin(size_type n) const {
        return const_local_iterator(buckets_[n]);
    }

    local_iterator end(size_type) {
        return local_iterator(nullptr);
    }

    const_local_iterator end(size_type) const {
        return const_local_iterator(nullptr);
    }

    const_local_iterator cend(size_type) const {
        return const_local_iterator(nullptr);
    }

    // 元素操作
    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        // 检查是否需要rehash
        if (size_ + 1 > bucket_count_ * max_load_factor_) {
            rehash(bucket_count_ * 2);
        }

        size_type bucket_idx = hash_to_bucket(value);
        Node* current = buckets_[bucket_idx];

        // 检查元素是否已存在
        while (current) {
            if (key_equal_(current->value, value)) {
                return {iterator(current, this, bucket_idx), false};
            }
            current = current->next;
        }

        // 创建新节点并插入到链表头部
        Node* new_node = new Node(value);
        new_node->next = buckets_[bucket_idx];
        buckets_[bucket_idx] = new_node;
        ++size_;

        return {iterator(new_node, this, bucket_idx), true};
    }

    std::pair<iterator, bool> insert(value_type&& value) {
        // 检查是否需要rehash
        if (size_ + 1 > bucket_count_ * max_load_factor_) {
            rehash(bucket_count_ * 2);
        }

        size_type bucket_idx = hash_to_bucket(value);
        Node* current = buckets_[bucket_idx];

        // 检查元素是否已存在
        while (current) {
            if (key_equal_(current->value, value)) {
                return {iterator(current, this, bucket_idx), false};
            }
            current = current->next;
        }

        // 创建新节点并插入到链表头部
        Node* new_node = new Node(std::move(value));
        new_node->next = buckets_[bucket_idx];
        buckets_[bucket_idx] = new_node;
        ++size_;

        return {iterator(new_node, this, bucket_idx), true};
    }

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        return insert(value_type(std::forward<Args>(args)...));
    }

    template<typename... Args>
    std::pair<iterator, bool> try_emplace(const key_type& key, Args&&... args) {
        // 首先检查元素是否已存在
        auto it = find(key);
        if (it != end()) {
            return {it, false};
        }

        // 如果元素不存在，则构造新元素
        return emplace(std::forward<Args>(args)...);
    }

    template<typename... Args>
    std::pair<iterator, bool> try_emplace(key_type&& key, Args&&... args) {
        // 首先检查元素是否已存在
        auto it = find(key);
        if (it != end()) {
            return {it, false};
        }

        // 如果元素不存在，则构造新元素
        return emplace(std::forward<Args>(args)...);
    }

    template<typename... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) {
        // 在unordered_set中，hint通常被忽略，因为位置由哈希函数决定
        // 但我们可以使用hint的bucket_idx_作为起点进行查找，可能会稍微提高性能
        auto result = emplace(std::forward<Args>(args)...);
        return result.first;
    }

    iterator erase(iterator pos) {
        if (pos == end()) {
            return end();
        }

        size_type bucket_idx = pos.bucket_idx_;
        Node* current = buckets_[bucket_idx];
        Node* prev = nullptr;
        Node* target = pos.node_;
        iterator next = pos;
        ++next;

        while (current) {
            if (current == target) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    buckets_[bucket_idx] = current->next;
                }
                delete current;
                --size_;
                return next;
            }
            prev = current;
            current = current->next;
        }

        return end(); // 不应该到达这里
    }

    iterator erase(iterator first, iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return last;
    }

    size_type erase(const key_type& key) {
        if (bucket_count_ == 0) return 0;

        size_type bucket_idx = hash_to_bucket(key);
        Node* current = buckets_[bucket_idx];
        Node* prev = nullptr;

        while (current) {
            if (key_equal_(current->value, key)) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    buckets_[bucket_idx] = current->next;
                }
                delete current;
                --size_;
                return 1;
            }
            prev = current;
            current = current->next;
        }

        return 0;
    }

    // 提取节点
    node_type extract(const_iterator position) {
        if (position == end()) {
            return node_type();
        }

        // 找到节点在桶中的位置
        size_type bucket_idx = hash_to_bucket(*position);
        Node* current = buckets_[bucket_idx];
        Node* prev = nullptr;

        // 在桶中查找节点
        while (current && current != position.node_) {
            prev = current;
            current = current->next;
        }

        if (!current) {
            return node_type();  // 未找到节点
        }

        // 从链表中移除节点
        if (prev) {
            prev->next = current->next;
        } else {
            buckets_[bucket_idx] = current->next;
        }

        current->next = nullptr;  // 断开节点与链表的连接
        --size_;

        return node_type(current);
    }

    node_type extract(const key_type& key) {
        auto it = find(key);
        return extract(it);
    }

    iterator find(const key_type& key) {
        Node* node = find_node(key);
        if (node) {
            return iterator(node, this, hash_to_bucket(key));
        }
        return end();
    }

    const_iterator find(const key_type& key) const {
        const Node* node = find_node(key);
        if (node) {
            return const_iterator(node, this, hash_to_bucket(key));
        }
        return cend();
    }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
        iterator it = find(key);
        if (it == end()) {
            return {it, it};
        }
        iterator next = it;
        ++next;
        return {it, next};
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
        const_iterator it = find(key);
        if (it == cend()) {
            return {it, it};
        }
        const_iterator next = it;
        ++next;
        return {it, next};
    }

    size_type count(const key_type& key) const {
        return find_node(key) ? 1 : 0;
    }

    bool contains(const key_type& key) const {
        return find_node(key) != nullptr;
    }

    void clear() {
        for (size_type i = 0; i < bucket_count_; ++i) {
            Node* current = buckets_[i];
            while (current) {
                Node* next = current->next;
                delete current;
                current = next;
            }
            buckets_[i] = nullptr;
        }
        size_ = 0;
    }

    void rehash(size_type count) {
        size_type new_bucket_count = std::max(count, size_type(1));
        
        // 如果新的桶数量小于当前元素数量除以最大负载因子，调整桶数量
        if (new_bucket_count < size_ / max_load_factor_) {
            new_bucket_count = std::ceil(size_ / max_load_factor_);
        }

        // 避免过度收缩，确保新的桶数量不小于当前桶数量的一半
        // 除非新的桶数量是1（最小值）
        if (new_bucket_count < bucket_count_ / 2 && new_bucket_count > 1) {
            new_bucket_count = bucket_count_ / 2;
        }

        // 如果新的桶数量与当前相同，不需要rehash
        if (new_bucket_count == bucket_count_) {
            return;
        }

        // 分配新的桶数组
        Node** new_buckets = new Node*[new_bucket_count]();

        // 重新哈希所有元素
        for (size_type i = 0; i < bucket_count_; ++i) {
            Node* current = buckets_[i];
            while (current) {
                Node* next = current->next;
                
                // 计算新的桶索引
                size_type new_bucket = hash_function_(current->value) % new_bucket_count;
                
                // 将节点插入新的桶
                current->next = new_buckets[new_bucket];
                new_buckets[new_bucket] = current;
                
                current = next;
            }
        }

        // 释放旧的桶数组
        delete[] buckets_;

        // 更新成员变量
        buckets_ = new_buckets;
        bucket_count_ = new_bucket_count;
    }

    void reserve(size_type count) {
        rehash(std::ceil(count / max_load_factor_));
    }

    // 桶接口
    size_type max_bucket_count() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    size_type bucket_size(size_type n) const {
        size_type count = 0;
        for (Node* current = buckets_[n]; current; current = current->next) {
            ++count;
        }
        return count;
    }

    size_type bucket(const key_type& key) const {
        return hash_to_bucket(key);
    }

    // 观察器
    hasher hash_function() const {
        return hash_function_;
    }

    key_equal key_eq() const {
        return key_equal_;
    }

    // 合并操作
    void merge(unordered_set& other) {
        if (this == &other) {
            return;
        }

        // 预留足够的空间
        reserve(size_ + other.size_);

        // 遍历other中的所有元素
        for (auto it = other.begin(); it != other.end(); ) {
            auto current = it++;
            auto key = *current;
            
            // 如果当前容器中不存在该元素，则移动它
            if (!contains(key)) {
                insert(std::move(key));
                other.erase(current);
            }
        }
    }

    // 辅助功能
    void swap(unordered_set& other) noexcept {
        std::swap(buckets_, other.buckets_);
        std::swap(bucket_count_, other.bucket_count_);
        std::swap(size_, other.size_);
        std::swap(max_load_factor_, other.max_load_factor_);
        std::swap(hash_function_, other.hash_function_);
        std::swap(key_equal_, other.key_equal_);
    }
};

// 非成员函数
template <typename Key, typename Hash, typename KeyEqual>
bool operator==(const unordered_set<Key, Hash, KeyEqual>& lhs,
                const unordered_set<Key, Hash, KeyEqual>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (const auto& value : lhs) {
        if (rhs.find(value) == rhs.end()) {
            return false;
        }
    }

    return true;
}

template <typename Key, typename Hash, typename KeyEqual>
bool operator!=(const unordered_set<Key, Hash, KeyEqual>& lhs,
                const unordered_set<Key, Hash, KeyEqual>& rhs) {
    return !(lhs == rhs);
}

template <typename Key, typename Hash, typename KeyEqual>
void swap(unordered_set<Key, Hash, KeyEqual>& lhs,
          unordered_set<Key, Hash, KeyEqual>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace sjkxq_stl

#endif // SJKXQ_STL_UNORDERED_SET_HPP