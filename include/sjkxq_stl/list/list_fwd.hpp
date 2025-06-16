#ifndef SJKXQ_STL_LIST_FWD_HPP
#define SJKXQ_STL_LIST_FWD_HPP

#include <memory>

namespace sjkxq_stl {

// 前向声明
template <typename T, typename Allocator = std::allocator<T>>
class list;

template <typename T>
struct list_node_impl;

template <typename T>
class list_iterator;

template <typename T>
class const_list_iterator;

template <typename T, typename Allocator>
class list_node_allocator;

} // namespace sjkxq_stl

#endif // SJKXQ_STL_LIST_FWD_HPP