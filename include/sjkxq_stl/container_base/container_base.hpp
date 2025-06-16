#ifndef SJKXQ_STL_CONTAINER_BASE_HPP
#define SJKXQ_STL_CONTAINER_BASE_HPP

#include "../common.hpp"
#include "node_base.hpp"
#include <memory>
#include <type_traits>

namespace sjkxq_stl {

// 容器基类，提供一些通用功能
template <typename Container>
class container_base {
protected:
    using size_type = typename Container::size_type;
    using difference_type = typename Container::difference_type;

    // 检查索引是否越界
    void range_check(size_type pos, size_type size) const {
        if (pos >= size)
            throw out_of_range("container index out of range");
    }

    // 检查迭代器是否属于同一个容器
    template <typename Iterator1, typename Iterator2>
    static bool iterator_in_same_container(const Iterator1& it1, const Iterator2& it2) {
        return it1.node->next->prev == it2.node->prev->next;
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_CONTAINER_BASE_HPP