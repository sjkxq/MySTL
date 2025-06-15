#ifndef SJKXQ_STL_QUEUE_HPP
#define SJKXQ_STL_QUEUE_HPP

#include "common.hpp"
#include <deque>

namespace sjkxq_stl
{

template <typename T, typename Container = std::deque<T>>
class queue
{
private:
  // 检查容器是否支持必需的操作
  static_assert(std::is_same<T, typename Container::value_type>::value,
                "Container::value_type must be the same as T");
  
  // 使用 SFINAE 检查容器是否支持必需的操作
  template<typename C>
  struct has_empty {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().empty(), std::true_type());
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<C>(0))::value;
  };
  
  template<typename C>
  struct has_size {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().size(), std::true_type());
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<C>(0))::value;
  };
  
  template<typename C>
  struct has_front {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().front(), std::true_type());
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<C>(0))::value;
  };
  
  template<typename C>
  struct has_back {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().back(), std::true_type());
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<C>(0))::value;
  };
  
  template<typename C>
  struct has_push_back {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().push_back(std::declval<const T&>()), std::true_type());
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<C>(0))::value;
  };
  
  template<typename C>
  struct has_pop_front {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().pop_front(), std::true_type());
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<C>(0))::value;
  };
  
  static_assert(has_empty<Container>::value, "Container must support empty()");
  static_assert(has_size<Container>::value, "Container must support size()");
  static_assert(has_front<Container>::value, "Container must support front()");
  static_assert(has_back<Container>::value, "Container must support back()");
  static_assert(has_push_back<Container>::value, "Container must support push_back()");
  static_assert(has_pop_front<Container>::value, "Container must support pop_front()");

public:
  // 类型定义
  using container_type  = Container;
  using value_type      = typename Container::value_type;
  using size_type       = typename Container::size_type;
  using reference       = typename Container::reference;
  using const_reference = typename Container::const_reference;

protected:
  Container c;  // 底层容器

public:
  // 构造函数
  queue() : c() {}

  explicit queue(const Container& cont) : c(cont) {}

  explicit queue(Container&& cont) : c(std::move(cont)) {}

  queue(const queue& other) : c(other.c) {}

  queue(queue&& other) noexcept : c(std::move(other.c)) {}

  template <typename Alloc>
  explicit queue(const Alloc& alloc) : c(alloc)
  {
  }

  template <typename Alloc>
  queue(const Container& cont, const Alloc& alloc) : c(cont, alloc)
  {
  }

  template <typename Alloc>
  queue(Container&& cont, const Alloc& alloc) : c(std::move(cont), alloc)
  {
  }

  template <typename Alloc>
  queue(const queue& other, const Alloc& alloc) : c(other.c, alloc)
  {
  }

  template <typename Alloc>
  queue(queue&& other, const Alloc& alloc) : c(std::move(other.c), alloc)
  {
  }

  // 赋值运算符
  queue& operator=(const queue& other)
  {
    c = other.c;
    return *this;
  }

  queue& operator=(queue&& other) noexcept
  {
    c = std::move(other.c);
    return *this;
  }

  // 元素访问
  reference front() {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    return c.front();
  }

  const_reference front() const {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    return c.front();
  }

  reference back() {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    return c.back();
  }

  const_reference back() const {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    return c.back();
  }

  // 容量
  bool empty() const { return c.empty(); }

  size_type size() const { return c.size(); }

  // 修改器
  void push(const value_type& value) { c.push_back(value); }

  void push(value_type&& value) { c.push_back(std::move(value)); }

  // 检查容器是否支持 emplace_back
  template<typename C, typename... Args>
  struct has_emplace_back {
    template<typename U>
    static auto test(int) -> decltype(std::declval<U>().emplace_back(std::declval<Args>()...), std::true_type());
    template<typename>
    static std::false_type test(...);
    static constexpr bool value = decltype(test<C>(0))::value;
  };

  // 使用 SFINAE 实现 emplace
  template <typename... Args>
  void emplace(Args&&... args)
  {
    emplace_impl(std::integral_constant<bool, has_emplace_back<Container, Args...>::value>{}, 
                 std::forward<Args>(args)...);
  }

  // 如果容器支持 emplace_back
  template <typename... Args>
  void emplace_impl(std::true_type, Args&&... args)
  {
    c.emplace_back(std::forward<Args>(args)...);
  }

  // 如果容器不支持 emplace_back
  template <typename... Args>
  void emplace_impl(std::false_type, Args&&... args)
  {
    c.push_back(value_type(std::forward<Args>(args)...));
  }

  void pop() {
    if (empty()) {
      throw std::out_of_range("Queue is empty");
    }
    c.pop_front();
  }

  void swap(queue& other) noexcept(std::is_nothrow_swappable_v<Container>)
  {
    using std::swap;
    swap(c, other.c);
  }

  // 比较运算符
  friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.c == rhs.c; }

  friend bool operator!=(const queue& lhs, const queue& rhs) { return lhs.c != rhs.c; }

  friend bool operator<(const queue& lhs, const queue& rhs) { return lhs.c < rhs.c; }

  friend bool operator<=(const queue& lhs, const queue& rhs) { return lhs.c <= rhs.c; }

  friend bool operator>(const queue& lhs, const queue& rhs) { return lhs.c > rhs.c; }

  friend bool operator>=(const queue& lhs, const queue& rhs) { return lhs.c >= rhs.c; }
};

// 特化的swap函数
template <typename T, typename Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}

}  // namespace sjkxq_stl

#endif  // SJKXQ_STL_QUEUE_HPP