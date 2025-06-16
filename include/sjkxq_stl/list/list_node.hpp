#ifndef SJKXQ_STL_LIST_NODE_HPP
#define SJKXQ_STL_LIST_NODE_HPP

#include "../container_base/node_base.hpp"
#include "../container_base/memory_base.hpp"
#include "list_fwd.hpp"
#include <memory>

namespace sjkxq_stl {

// 定义list节点
template <typename T>
struct list_node_impl : public node_base {
    T value;
    
    template <typename... Args>
    list_node_impl(Args&&... args) : value(std::forward<Args>(args)...) {}
};

// 为了保持向后兼容性，保留list_node_allocator，但使用新的基础设施
template <typename T, typename Allocator>
class list_node_allocator {
    using node_type = list_node_impl<T>;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<node_type>;
    using allocator_traits = std::allocator_traits<allocator_type>;

    allocator_type alloc;

public:
    using size_type = typename allocator_traits::size_type;

    explicit list_node_allocator(const Allocator& a = Allocator()) 
        : alloc(a) {}

    node_type* allocate(size_type n = 1) {
        return allocator_traits::allocate(alloc, n);
    }

    void deallocate(node_type* p, size_type n = 1) {
        allocator_traits::deallocate(alloc, p, n);
    }

    template <typename... Args>
    void construct(node_type* p, Args&&... args) {
        allocator_traits::construct(alloc, p, std::forward<Args>(args)...);
    }

    void destroy(node_type* p) {
        allocator_traits::destroy(alloc, p);
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_LIST_NODE_HPP