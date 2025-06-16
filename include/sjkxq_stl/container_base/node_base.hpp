#ifndef SJKXQ_STL_NODE_BASE_HPP
#define SJKXQ_STL_NODE_BASE_HPP

namespace sjkxq_stl {

// 基础节点结构，用于双向链表
struct node_base {
    node_base* prev;
    node_base* next;

    node_base() noexcept : prev(this), next(this) {}

    void hook(node_base* pos) noexcept {
        this->next = pos;
        this->prev = pos->prev;
        pos->prev->next = this;
        pos->prev = this;
    }

    void unhook() noexcept {
        node_base* const next_node = this->next;
        node_base* const prev_node = this->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }

    void transfer(node_base* first, node_base* last) noexcept {
        if (this != last) {
            // 从原位置移除
            last->prev->next = this;
            first->prev->next = last;
            this->prev->next = first;

            // 更新prev指针
            node_base* const tmp = this->prev;
            this->prev = last->prev;
            last->prev = first->prev;
            first->prev = tmp;
        }
    }
};

} // namespace sjkxq_stl

#endif // SJKXQ_STL_NODE_BASE_HPP