#ifndef SJKXQ_STL_MEMORY_BASE_HPP
#define SJKXQ_STL_MEMORY_BASE_HPP

#include <memory>
#include <type_traits>

namespace sjkxq_stl {

// 内存分配器基类，提供通用功能
template <typename T, typename Allocator = std::allocator<T>>
class memory_base {
protected:
    using allocator_type = Allocator;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using value_type = typename allocator_traits::value_type;
    using pointer = typename allocator_traits::pointer;
    using const_pointer = typename allocator_traits::const_pointer;
    using size_type = typename allocator_traits::size_type;
    using difference_type = typename allocator_traits::difference_type;

    allocator_type alloc;

public:
    explicit memory_base(const allocator_type& a = allocator_type()) 
        : alloc(a) {}

    allocator_type get_allocator() const noexcept {
        return alloc;
    }

protected:
    pointer allocate(size_type n) {
        return allocator_traits::allocate(alloc, n);
    }

    void deallocate(pointer p, size_type n) {
        allocator_traits::deallocate(alloc, p, n);
    }

    template <typename... Args>
    void construct(pointer p, Args&&... args) {
        allocator_traits::construct(alloc, p, std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
        allocator_traits::destroy(alloc, p);
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_MEMORY_BASE_HPP