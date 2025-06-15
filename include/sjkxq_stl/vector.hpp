#ifndef SJKXQ_STL_VECTOR_HPP
#define SJKXQ_STL_VECTOR_HPP

#include "common.hpp"
#include <algorithm>
#include <initializer_list>
#include <memory>
#include <iterator> // 为移动迭代器支持

namespace sjkxq_stl
{

template <typename T, typename Allocator = std::allocator<T>>
class vector
{
public:
  // 类型定义
  using value_type             = T;
  using allocator_type         = Allocator;
  using size_type              = sjkxq_stl::size_type;
  using difference_type        = std::ptrdiff_t;
  using reference              = value_type&;
  using const_reference        = const value_type&;
  using pointer                = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer          = typename std::allocator_traits<Allocator>::const_pointer;
  using iterator               = pointer;
  using const_iterator         = const_pointer;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
  pointer        data_;      // 指向数据的指针
  size_type      size_;      // 当前元素数量
  size_type      capacity_;  // 当前容量
  allocator_type alloc_;     // 分配器

  // 重新分配内存
  void reallocate(size_type new_capacity)
  {
    if (new_capacity > max_size()) {
      throw std::length_error("vector::reallocate: capacity exceeds maximum size");
    }

    pointer new_data = alloc_.allocate(new_capacity);

    // 移动现有元素到新内存，使用异常安全的方式
    size_type i = 0;
    try {
      for (; i < size_; ++i) {
        std::allocator_traits<allocator_type>::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
      }
    } catch (...) {
      // 如果构造过程中发生异常，清理已构造的元素
      for (size_type j = 0; j < i; ++j) {
        std::allocator_traits<allocator_type>::destroy(alloc_, new_data + j);
      }
      alloc_.deallocate(new_data, new_capacity);
      throw;  // 重新抛出异常
    }

    // 注意：移动后的对象仍然需要被销毁，但它们现在处于有效但未指定的状态
    for (size_type j = 0; j < size_; ++j) {
      std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
    }

    // 释放旧内存
    if (data_) {
      alloc_.deallocate(data_, capacity_);
    }

    // 更新指针和容量
    data_     = new_data;
    capacity_ = new_capacity;
  }

public:
  // 构造函数
  vector() noexcept(noexcept(Allocator())) : data_(nullptr), size_(0), capacity_(0), alloc_() {}

  explicit vector(const Allocator& alloc) noexcept
      : data_(nullptr), size_(0), capacity_(0), alloc_(alloc)
  {
  }

  vector(size_type count, const T& value, const Allocator& alloc = Allocator())
      : data_(nullptr), size_(0), capacity_(0), alloc_(alloc)
  {
    if (count > 0) {
      data_     = alloc_.allocate(count);
      capacity_ = count;
      size_     = count;

      for (size_type i = 0; i < count; ++i) {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
      }
    }
  }

  explicit vector(size_type count, const Allocator& alloc = Allocator())
      : data_(nullptr), size_(0), capacity_(0), alloc_(alloc)
  {
    if (count > 0) {
      data_     = alloc_.allocate(count);
      capacity_ = count;
      size_     = count;

      for (size_type i = 0; i < count; ++i) {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + i);
      }
    }
  }

  vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
      : data_(nullptr), size_(0), capacity_(0), alloc_(alloc)
  {
    if (init.size() > 0) {
      data_     = alloc_.allocate(init.size());
      capacity_ = init.size();
      size_     = init.size();

      size_type i = 0;
      for (const auto& item : init) {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, item);
        ++i;
      }
    }
  }

  // 从迭代器范围构造，支持移动迭代器
  template<typename InputIt, typename = typename std::enable_if_t<!std::is_integral_v<InputIt>>>
  vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
      : data_(nullptr), size_(0), capacity_(0), alloc_(alloc)
  {
    // 对于随机访问迭代器，我们可以预先知道大小
    if constexpr (std::is_same_v<typename std::iterator_traits<InputIt>::iterator_category,
                                std::random_access_iterator_tag>) {
      auto count = std::distance(first, last);
      if (count > 0) {
        data_ = alloc_.allocate(count);
        capacity_ = count;
        size_ = count;

        size_type i = 0;
        try {
          for (auto it = first; it != last; ++it, ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, *it);
          }
        } catch (...) {
          // 清理已构造的元素
          for (size_type j = 0; j < i; ++j) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
          }
          alloc_.deallocate(data_, capacity_);
          throw;
        }
      }
    } else {
      // 对于其他类型的迭代器，我们需要逐个插入
      for (; first != last; ++first) {
        emplace_back(*first);
      }
    }
  }

  // 复制构造函数
  vector(const vector& other)
      : data_(nullptr), size_(0), capacity_(0),
        alloc_(std::allocator_traits<allocator_type>::select_on_container_copy_construction(
            other.alloc_))
  {
    if (other.size_ > 0) {
      data_     = alloc_.allocate(other.size_);
      capacity_ = other.size_;
      size_     = other.size_;

      for (size_type i = 0; i < size_; ++i) {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, other.data_[i]);
      }
    }
  }

  // 移动构造函数
  vector(vector&& other) noexcept
      : data_(other.data_), size_(other.size_), capacity_(other.capacity_),
        alloc_(std::move(other.alloc_))
  {
    other.data_     = nullptr;
    other.size_     = 0;
    other.capacity_ = 0;
  }

  // 析构函数
  ~vector()
  {
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
  vector& operator=(const vector& other)
  {
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
        data_     = alloc_.allocate(other.size_);
        capacity_ = other.size_;
        size_     = other.size_;

        // 复制元素
        for (size_type i = 0; i < size_; ++i) {
          std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, other.data_[i]);
        }
      } else {
        data_     = nullptr;
        size_     = 0;
        capacity_ = 0;
      }
    }
    return *this;
  }

  // 移动赋值运算符
  vector& operator=(vector&& other) noexcept
  {
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
      data_     = other.data_;
      size_     = other.size_;
      capacity_ = other.capacity_;

      // 重置other
      other.data_     = nullptr;
      other.size_     = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  // 元素访问
  reference at(size_type pos)
  {
    if (pos >= size_) {
      throw out_of_range("vector::at: pos (which is " + std::to_string(pos)
                         + ") >= this->size() (which is " + std::to_string(size_) + ")");
    }
    return data_[pos];
  }

  const_reference at(size_type pos) const
  {
    if (pos >= size_) {
      throw out_of_range("vector::at: pos (which is " + std::to_string(pos)
                         + ") >= this->size() (which is " + std::to_string(size_) + ")");
    }
    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }

  const_reference operator[](size_type pos) const { return data_[pos]; }

  reference front() {
    if (empty()) {
      throw std::out_of_range("vector::front: vector is empty");
    }
    return data_[0];
  }

  const_reference front() const {
    if (empty()) {
      throw std::out_of_range("vector::front: vector is empty");
    }
    return data_[0];
  }

  reference back() {
    if (empty()) {
      throw std::out_of_range("vector::back: vector is empty");
    }
    return data_[size_ - 1];
  }

  const_reference back() const {
    if (empty()) {
      throw std::out_of_range("vector::back: vector is empty");
    }
    return data_[size_ - 1];
  }

  pointer data() noexcept { return data_; }

  const_pointer data() const noexcept { return data_; }

  // 迭代器
  iterator begin() noexcept { return data_; }

  const_iterator begin() const noexcept { return data_; }

  const_iterator cbegin() const noexcept { return data_; }

  iterator end() noexcept { return data_ + size_; }

  const_iterator end() const noexcept { return data_ + size_; }

  const_iterator cend() const noexcept { return data_ + size_; }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

  const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

  const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

  const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

  // 容量
  bool empty() const noexcept { return size_ == 0; }

  size_type size() const noexcept { return size_; }

  size_type max_size() const noexcept
  {
    return std::min(
      std::allocator_traits<allocator_type>::max_size(alloc_),
      static_cast<size_type>(std::numeric_limits<difference_type>::max() - 1)  // 减1，确保小于最大值
    );
  }

  void reserve(size_type new_cap)
  {
    if (new_cap > capacity_) {
      reallocate(new_cap);
    }
  }

  size_type capacity() const noexcept { return capacity_; }

  void shrink_to_fit()
  {
    if (size_ < capacity_) {
      reallocate(size_);
    }
  }

  // 修改器
  void clear() noexcept
  {
    for (size_type i = size_; i > 0; --i) {
      std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i - 1);
    }
    size_ = 0;
  }

  // 用指定数量的元素替换内容
  void assign(size_type count, const T& value)
  {
    clear();
    if (count > capacity_) {
      if (data_) {
        alloc_.deallocate(data_, capacity_);
      }
      data_ = alloc_.allocate(count);
      capacity_ = count;
    }

    for (size_type i = 0; i < count; ++i) {
      std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
    }
    size_ = count;
  }

  // 用范围[first, last)中的元素替换内容，支持移动迭代器
  template<typename InputIt, typename = typename std::enable_if_t<!std::is_integral_v<InputIt>>>
  void assign(InputIt first, InputIt last)
  {
    clear();
    
    // 对于随机访问迭代器，我们可以预先知道大小
    if constexpr (std::is_same_v<typename std::iterator_traits<InputIt>::iterator_category,
                                std::random_access_iterator_tag>) {
      auto count = std::distance(first, last);
      if (count > 0) {
        if (static_cast<size_type>(count) > capacity_) {
          if (data_) {
            alloc_.deallocate(data_, capacity_);
          }
          data_ = alloc_.allocate(count);
          capacity_ = count;
        }
        
        size_type i = 0;
        try {
          for (auto it = first; it != last; ++it, ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, *it);
          }
        } catch (...) {
          // 清理已构造的元素
          for (size_type j = 0; j < i; ++j) {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
          }
          size_ = 0;
          throw;
        }
        size_ = count;
      }
    } else {
      // 对于其他类型的迭代器，我们需要逐个插入
      for (; first != last; ++first) {
        push_back(*first);
      }
    }
  }

  // 用初始化列表中的元素替换内容
  void assign(std::initializer_list<T> ilist)
  {
    assign(ilist.begin(), ilist.end());
  }

  void push_back(const T& value)
  {
    if (size_ == capacity_) {
      size_type new_capacity = calculate_growth(size_ + 1);
      reallocate(new_capacity);
    }
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, value);
    ++size_;
  }

  void push_back(T&& value)
  {
    if (size_ == capacity_) {
      size_type new_capacity = calculate_growth(size_ + 1);
      reallocate(new_capacity);
    }
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, std::forward<T>(value));
    ++size_;
  }

  void pop_back()
  {
    if (empty()) {
      throw std::out_of_range("vector::pop_back: vector is empty");
    }
    --size_;
    std::allocator_traits<allocator_type>::destroy(alloc_, data_ + size_);
  }

  // emplace_back：在容器末尾就地构造元素
  template<typename... Args>
  reference emplace_back(Args&&... args)
  {
    if (size_ == capacity_) {
      size_type new_capacity = calculate_growth(size_ + 1);
      reallocate(new_capacity);
    }
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + size_, std::forward<Args>(args)...);
    ++size_;
    return back();
  }

  // emplace：在指定位置就地构造元素
  template<typename... Args>
  iterator emplace(const_iterator pos, Args&&... args)
  {
    // 计算插入位置的索引
    size_type index = pos - begin();
    if (index > size_) {
      throw std::out_of_range("vector::emplace: position out of range");
    }

    if (size_ == capacity_) {
      // 需要重新分配内存
      size_type new_capacity = calculate_growth(size_ + 1);
      pointer new_data = alloc_.allocate(new_capacity);

      // 移动前半部分元素
      size_type i = 0;
      try {
        for (; i < index; ++i) {
          std::allocator_traits<allocator_type>::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
        }
        // 在指定位置构造新元素
        std::allocator_traits<allocator_type>::construct(alloc_, new_data + index, std::forward<Args>(args)...);
        // 移动后半部分元素
        for (; i < size_; ++i) {
          std::allocator_traits<allocator_type>::construct(alloc_, new_data + i + 1, std::move_if_noexcept(data_[i]));
        }
      } catch (...) {
        // 清理已构造的元素
        for (size_type j = 0; j <= i; ++j) {
          std::allocator_traits<allocator_type>::destroy(alloc_, new_data + j);
        }
        alloc_.deallocate(new_data, new_capacity);
        throw;
      }

      // 清理旧数据
      for (size_type j = 0; j < size_; ++j) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
      }
      if (data_) {
        alloc_.deallocate(data_, capacity_);
      }

      data_ = new_data;
      capacity_ = new_capacity;
    } else {
      // 有足够空间，不需要重新分配
      if (index < size_) {
        // 移动后面的元素，为新元素腾出空间
        for (size_type i = size_; i > index; --i) {
          std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move_if_noexcept(data_[i - 1]));
          std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i - 1);
        }
      }
      // 在指定位置构造新元素
      std::allocator_traits<allocator_type>::construct(alloc_, data_ + index, std::forward<Args>(args)...);
    }
    ++size_;
    return begin() + index;
  }

  // 在指定位置插入单个元素
  iterator insert(const_iterator pos, const T& value)
  {
    return emplace(pos, value);
  }

  // 在指定位置插入单个元素（移动版本）
  iterator insert(const_iterator pos, T&& value)
  {
    return emplace(pos, std::move(value));
  }

  // 在指定位置插入count个相同的元素
  iterator insert(const_iterator pos, size_type count, const T& value)
  {
    if (count == 0) {
      return const_cast<iterator>(pos);
    }

    // 计算插入位置的索引
    size_type index = pos - begin();
    if (index > size_) {
      throw std::out_of_range("vector::insert: position out of range");
    }

    // 检查是否需要重新分配内存
    if (size_ + count > capacity_) {
      size_type new_capacity = calculate_growth(size_ + count);
      pointer new_data = alloc_.allocate(new_capacity);

      // 移动前半部分元素
      size_type i = 0;
      try {
        for (; i < index; ++i) {
          std::allocator_traits<allocator_type>::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
        }
        // 在指定位置构造新元素
        for (size_type j = 0; j < count; ++j) {
          std::allocator_traits<allocator_type>::construct(alloc_, new_data + index + j, value);
        }
        // 移动后半部分元素
        for (; i < size_; ++i) {
          std::allocator_traits<allocator_type>::construct(alloc_, new_data + i + count, std::move_if_noexcept(data_[i]));
        }
      } catch (...) {
        // 清理已构造的元素
        for (size_type j = 0; j < i + count; ++j) {
          std::allocator_traits<allocator_type>::destroy(alloc_, new_data + j);
        }
        alloc_.deallocate(new_data, new_capacity);
        throw;
      }

      // 清理旧数据
      for (size_type j = 0; j < size_; ++j) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
      }
      if (data_) {
        alloc_.deallocate(data_, capacity_);
      }

      data_ = new_data;
      capacity_ = new_capacity;
    } else {
      // 有足够空间，不需要重新分配
      // 从后向前移动元素，为新元素腾出空间
      for (size_type i = size_ + count - 1; i >= index + count; --i) {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move_if_noexcept(data_[i - count]));
      }

      // 销毁将被覆盖的元素
      for (size_type i = size_ - 1; i >= index && i < size_; --i) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
      }

      // 在指定位置构造新元素
      for (size_type i = 0; i < count; ++i) {
        std::allocator_traits<allocator_type>::construct(alloc_, data_ + index + i, value);
      }
    }

    size_ += count;
    return begin() + index;
  }

  // 在指定位置插入范围[first, last)中的元素，支持移动迭代器
  template<typename InputIt, typename = typename std::enable_if_t<!std::is_integral_v<InputIt>>>
  iterator insert(const_iterator pos, InputIt first, InputIt last)
  {
    // 计算插入位置的索引
    size_type index = pos - begin();
    if (index > size_) {
      throw std::out_of_range("vector::insert: position out of range");
    }

    // 对于随机访问迭代器，我们可以预先知道大小
    if constexpr (std::is_same_v<typename std::iterator_traits<InputIt>::iterator_category,
                                std::random_access_iterator_tag>) {
      auto count = std::distance(first, last);
      if (count <= 0) {
        return begin() + index;
      }

      // 检查是否需要重新分配内存
      if (size_ + count > capacity_) {
        size_type new_capacity = calculate_growth(size_ + count);
        pointer new_data = alloc_.allocate(new_capacity);

        // 移动前半部分元素
        size_type i = 0;
        try {
          for (; i < index; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
          }
          // 在指定位置构造新元素
          size_type j = 0;
          for (auto it = first; it != last; ++it, ++j) {
            std::allocator_traits<allocator_type>::construct(alloc_, new_data + index + j, *it);
          }
          // 移动后半部分元素
          for (; i < size_; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, new_data + i + count, std::move_if_noexcept(data_[i]));
          }
        } catch (...) {
          // 清理已构造的元素
          for (size_type j = 0; j < i + count; ++j) {
            std::allocator_traits<allocator_type>::destroy(alloc_, new_data + j);
          }
          alloc_.deallocate(new_data, new_capacity);
          throw;
        }

        // 清理旧数据
        for (size_type j = 0; j < size_; ++j) {
          std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
        }
        if (data_) {
          alloc_.deallocate(data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_capacity;
      } else {
        // 有足够空间，不需要重新分配
        // 从后向前移动元素，为新元素腾出空间
        for (size_type i = size_ + count - 1; i >= index + count; --i) {
          std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move_if_noexcept(data_[i - count]));
        }

        // 销毁将被覆盖的元素
        for (size_type i = size_ - 1; i >= index && i < size_; --i) {
          std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }

        // 在指定位置构造新元素
        size_type j = 0;
        for (auto it = first; it != last; ++it, ++j) {
          std::allocator_traits<allocator_type>::construct(alloc_, data_ + index + j, *it);
        }
      }

      size_ += count;
    } else {
      // 对于其他类型的迭代器，我们需要先收集所有元素
      std::vector<T> temp(first, last);
      return insert(pos, std::make_move_iterator(temp.begin()), std::make_move_iterator(temp.end()));
    }

    return begin() + index;
  }

  // 在指定位置插入初始化列表中的元素
  iterator insert(const_iterator pos, std::initializer_list<T> ilist)
  {
    return insert(pos, ilist.begin(), ilist.end());
  }

  void resize(size_type count)
  {
    if (count > size_) {
      reserve(count);  // 使用 reserve 来处理内存分配
      
      // 使用异常安全的方式构造新元素
      size_type i = size_;
      try {
        for (; i < count; ++i) {
          std::allocator_traits<allocator_type>::construct(alloc_, data_ + i);
        }
      } catch (...) {
        // 如果构造过程中发生异常，清理已构造的元素
        for (size_type j = size_; j < i; ++j) {
          std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
        }
        throw;  // 重新抛出异常
      }
      size_ = count;
    } else if (count < size_) {
      // 从后向前销毁元素，这样可以保证在部分销毁时的一致性
      for (size_type i = size_ - 1; i >= count; --i) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
      }
      size_ = count;
    }
  }

  void resize(size_type count, const value_type& value)
  {
    if (count > size_) {
      reserve(count);  // 使用 reserve 来处理内存分配
      
      // 使用异常安全的方式构造新元素
      size_type i = size_;
      try {
        for (; i < count; ++i) {
          std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, value);
        }
      } catch (...) {
        // 如果构造过程中发生异常，清理已构造的元素
        for (size_type j = size_; j < i; ++j) {
          std::allocator_traits<allocator_type>::destroy(alloc_, data_ + j);
        }
        throw;  // 重新抛出异常
      }
      size_ = count;
    } else if (count < size_) {
      // 从后向前销毁元素，这样可以保证在部分销毁时的一致性
      for (size_type i = size_ - 1; i >= count; --i) {
        std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
      }
      size_ = count;
    }
  }

private:
  // 计算增长后的容量
  // 使用更高效的增长策略：
  // 1. 对于小容量，使用更激进的增长因子(2倍)以减少重新分配次数
  // 2. 对于大容量，使用更保守的增长因子(1.5倍)以减少内存浪费
  // 3. 确保增长后的容量至少能容纳请求的大小
  size_type calculate_growth(size_type new_size) const {
    const size_type ms = max_size();
    if (new_size > ms) {
      throw std::length_error("vector::calculate_growth: size exceeds maximum size");
    }
    
    const size_type cap = capacity();
    if (cap >= ms / 2) {
      return ms;  // 已经达到最大容量的一半，直接返回最大容量
    }

    // 小容量时使用2倍增长，大容量时使用1.5倍增长
    size_type growth;
    if (cap < 4096) {
      growth = cap * 2;  // 小容量时翻倍增长
    } else {
      growth = cap + cap / 2;  // 大容量时增加50%
    }

    // 确保至少能容纳请求的大小，并且不超过最大容量
    return std::min(
      std::max(
        std::max(growth, new_size),  // 至少是增长后的容量或满足新大小
        size_type(16)                // 最小初始容量为16
      ),
      ms
    );
  }

  // 比较运算符
  friend bool operator==(const vector& lhs, const vector& rhs)
  {
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

  friend bool operator!=(const vector& lhs, const vector& rhs) { return !(lhs == rhs); }

  friend bool operator<(const vector& lhs, const vector& rhs)
  {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
  }

  friend bool operator<=(const vector& lhs, const vector& rhs) { return !(rhs < lhs); }

  friend bool operator>(const vector& lhs, const vector& rhs) { return rhs < lhs; }

  friend bool operator>=(const vector& lhs, const vector& rhs) { return !(lhs < rhs); }

  // 交换两个vector的内容
  void swap(vector& other) noexcept(std::allocator_traits<allocator_type>::propagate_on_container_swap::value ||
                                   std::allocator_traits<allocator_type>::is_always_equal::value)
  {
    if (this != &other) {
      using std::swap;
      
      if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
        swap(alloc_, other.alloc_);
      } else {
        // 如果分配器不可交换，则要求它们必须相等
        if (alloc_ != other.alloc_) {
          throw std::runtime_error("vector::swap: allocators must be equal for containers with allocators that do not propagate on swap");
        }
      }
      
      swap(data_, other.data_);
      swap(size_, other.size_);
      swap(capacity_, other.capacity_);
    }
  }

  // 删除指定位置的元素
  iterator erase(const_iterator pos)
  {
    if (pos < cbegin() || pos >= cend()) {
      throw std::out_of_range("vector::erase: iterator out of range");
    }

    size_type index = pos - cbegin();
    
    // 移动后面的元素
    for (size_type i = index; i < size_ - 1; ++i) {
      std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
      std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, std::move_if_noexcept(data_[i + 1]));
    }
    
    // 销毁最后一个元素
    std::allocator_traits<allocator_type>::destroy(alloc_, data_ + size_ - 1);
    --size_;
    
    return begin() + index;
  }

  // 删除[first, last)范围内的元素
  iterator erase(const_iterator first, const_iterator last)
  {
    if (first > last || first < cbegin() || last > cend()) {
      throw std::out_of_range("vector::erase: iterators out of range");
    }

    if (first == last) {
      return const_cast<iterator>(first);
    }

    size_type start_index = first - cbegin();
    size_type count = last - first;
    
    // 如果删除范围为空或起始位置已经是末尾，直接返回
    if (count == 0 || start_index >= size_) {
      return begin() + start_index;
    }

    // 移动元素
    size_type elements_after = size_ - (start_index + count);
    for (size_type i = 0; i < elements_after; ++i) {
      size_type from_index = start_index + count + i;
      size_type to_index = start_index + i;
      
      std::allocator_traits<allocator_type>::destroy(alloc_, data_ + to_index);
      std::allocator_traits<allocator_type>::construct(alloc_, data_ + to_index, 
                                                      std::move_if_noexcept(data_[from_index]));
    }

    // 销毁多余的元素
    for (size_type i = size_ - count; i < size_; ++i) {
      std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
    }

    size_ -= count;
    return begin() + start_index;
  }

  // 非成员函数swap
  friend void swap(vector& lhs, vector& rhs) noexcept(noexcept(lhs.swap(rhs)))
  {
    lhs.swap(rhs);
  }

  // 返回容器的分配器
  allocator_type get_allocator() const noexcept { return alloc_; }
};

// C++17 非成员函数
template <typename T, typename Alloc>
typename vector<T, Alloc>::pointer data(vector<T, Alloc>& v) noexcept
{
  return v.data();
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::const_pointer data(const vector<T, Alloc>& v) noexcept
{
  return v.data();
}

template <typename T, typename Alloc>
bool empty(const vector<T, Alloc>& v) noexcept
{
  return v.empty();
}

template <typename T, typename Alloc>
typename vector<T, Alloc>::size_type size(const vector<T, Alloc>& v) noexcept
{
  return v.size();
}

}  // namespace sjkxq_stl

#endif  // SJKXQ_STL_VECTOR_HPP