#ifndef SJKXQ_STL_VECTOR_HPP
#define SJKXQ_STL_VECTOR_HPP

#include "common.hpp"
#include <algorithm>
#include <memory>
#include <initializer_list>

namespace sjkxq_stl {

template <typename T, typename Allocator = std::allocator<T>>
class vector {
public:
    // 类型定义
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = sjkxq_stl::size_type;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    pointer data_;           // 指向数据的指针
    size_type size_;         // 当前元素数量
    size_type capacity_;     // 当前容量
    allocator_type alloc_;   // 分配器

    // 重新分配内存
    void reallocate(size_type new_capacity) {
        pointer new_data = alloc_.allocate(new_capacity);
        
        // 移动现有元素到新内存
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, new_data + i, std::move(data_[i]));
        }
        
        // 销毁旧元素
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
        
        // 释放旧内存
        if (data_) {
            alloc_.deallocate(data_, capacity_);
        }
        
        // 更新指针和容量
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    // 构造函数
    vector() noexcept(noexcept(Allocator())) 
        : data_(nullptr), size_(0), capacity_(0), alloc_() {}
    
    explicit vector(const Allocator& alloc) noexcept 
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {}
    
    vector(size_type count, const T& value, const Allocator& alloc = Allocator())
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        if (count > 0) {
            data_ = alloc_.allocate(count);
            capacity_ = count;
            size_ = count;
            
            for (size_type i = 0; i < count; ++i) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
            }
        }
    }
    
    explicit vector(size_type count, const Allocator& alloc = Allocator())
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        if (count > 0) {
            data_ = alloc_.allocate(count);
            capacity_ = count;
            size_ = count;
            
            for (size_type i = 0; i < count; ++i) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i);
            }
        }
    }
    
    vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
        : data_(nullptr), size_(0), capacity_(0), alloc_(alloc) {
        if (init.size() > 0) {
            data_ = alloc_.allocate(init.size());
            capacity_ = init.size();
            size_ = init.size();
            
            size_type i = 0;
            for (const auto& item : init) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, item);
                ++i;
            }
        }
    }
    
    // 复制构造函数
    vector(const vector& other)
        : data_(nullptr), size_(0), capacity_(0), 
          alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.alloc_)) {
        if (other.size_ > 0) {
            data_ = alloc_.allocate(other.size_);
            capacity_ = other.size_;
            size_ = other.size_;
            
            for (size_type i = 0; i < size_; ++i) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, other.data_[i]);
            }
        }
    }
    
    // 移动构造函数
    vector(vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_), alloc_(std::move(other.alloc_)) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    
    // 析构函数
    ~vector() {
        if (data_) {
            // 销毁所有元素
            for (size_type i = 0; i < size_; ++i) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
            }
            
            // 释放内存
            alloc_.deallocate(data_, capacity_);
        }
    }
    
    // 赋值运算符
    vector& operator=(const vector& other) {
        if (this != &other) {
            // 清理当前内容
            for (size_type i = 0; i < size_; ++i) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
            }
            
            if (data_) {
                alloc_.deallocate(data_, capacity_);
            }
            
            // 复制分配器
            if (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
                alloc_ = other.alloc_;
            }
            
            // 分配新内存
            if (other.size_ > 0) {
                data_ = alloc_.allocate(other.size_);
                capacity_ = other.size_;
                size_ = other.size_;
                
                // 复制元素
                for (size_type i = 0; i < size_; ++i) {
                    std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, other.data_[i]);
                }
            } else {
                data_ = nullptr;
                size_ = 0;
                capacity_ = 0;
            }
        }
        return *this;
    }
    
    // 移动赋值运算符
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            // 清理当前内容
            for (size_type i = 0; i < size_; ++i) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
            }
            
            if (data_) {
                alloc_.deallocate(data_, capacity_);
            }
            
            // 移动分配器
            if (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
                alloc_ = std::move(other.alloc_);
            }
            
            // 移动数据
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            // 重置other
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    
    // 元素访问
    reference at(size_type pos) {
        if (pos >= size_) {
            throw out_of_range("vector::at: pos (which is " + std::to_string(pos) + 
                              ") >= this->size() (which is " + std::to_string(size_) + ")");
        }
        return data_[pos];
    }
    
    const_reference at(size_type pos) const {
        if (pos >= size_) {
            throw out_of_range("vector::at: pos (which is " + std::to_string(pos) + 
                              ") >= this->size() (which is " + std::to_string(size_) + ")");
        }
        return data_[pos];
    }
    
    reference operator[](size_type pos) {
        return data_[pos];
    }
    
    const_reference operator[](size_type pos) const {
        return data_[pos];
    }
    
    reference front() {
        return data_[0];
    }
    
    const_reference front() const {
        return data_[0];
    }
    
    reference back() {
        return data_[size_ - 1];
    }
    
    const_reference back() const {
        return data_[size_ - 1];
    }
    
    pointer data() noexcept {
        return data_;
    }
    
    const_pointer data() const noexcept {
        return data_;
    }
    
    // 迭代器
    iterator begin() noexcept {
        return data_;
    }
    
    const_iterator begin() const noexcept {
        return data_;
    }
    
    const_iterator cbegin() const noexcept {
        return data_;
    }
    
    iterator end() noexcept {
        return data_ + size_;
    }
    
    const_iterator end() const noexcept {
        return data_ + size_;
    }
    
    const_iterator cend() const noexcept {
        return data_ + size_;
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
        return std::allocator_traits<allocator_type>::max_size(alloc_);
    }
    
    void reserve(size_type new_cap) {
        if (new_cap > capacity_) {
            reallocate(new_cap);
        }
    }
    
    size_type capacity() const noexcept {
        return capacity_;
    }
    
    void shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_);
        }
    }
    
    // 修改器
    void clear() noexcept {
        for (size_type i = 0; i < size_; ++i) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
        size_ = 0;
    }
    
    void push_back(const T& value) {
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, value);
        ++size_;
    }
    
    void push_back(T&& value) {
        if (size_ == capacity_) {
            size_type new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, std::move(value));
        ++size_;
    }
    
    void pop_back() {
        if (size_ > 0) {
            --size_;
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + size_);
        }
    }
    
    void resize(size_type count) {
        if (count > size_) {
            if (count > capacity_) {
                reallocate(count);
            }
            for (size_type i = size_; i < count; ++i) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i);
            }
        } else if (count < size_) {
            for (size_type i = count; i < size_; ++i) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
            }
        }
        size_ = count;
    }
    
    void resize(size_type count, const value_type& value) {
        if (count > size_) {
            if (count > capacity_) {
                reallocate(count);
            }
            for (size_type i = size_; i < count; ++i) {
                std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
            }
        } else if (count < size_) {
            for (size_type i = count; i < size_; ++i) {
                std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
            }
        }
        size_ = count;
    }
    
    // 比较运算符
    friend bool operator==(const vector& lhs, const vector& rhs) {
        if (lhs.size_ != rhs.size_) {
            return false;
        }
        for (size_type i = 0; i < lhs.size_; ++i) {
            if (!(lhs.data_[i] == rhs.data_[i])) {
                return false;
            }
        }
        return true;
    }
    
    friend bool operator!=(const vector& lhs, const vector& rhs) {
        return !(lhs == rhs);
    }
    
    friend bool operator<(const vector& lhs, const vector& rhs) {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    
    friend bool operator<=(const vector& lhs, const vector& rhs) {
        return !(rhs < lhs);
    }
    
    friend bool operator>(const vector& lhs, const vector& rhs) {
        return rhs < lhs;
    }
    
    friend bool operator>=(const vector& lhs, const vector& rhs) {
        return !(lhs < rhs);
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_VECTOR_HPP