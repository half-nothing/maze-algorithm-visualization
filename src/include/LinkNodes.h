/**********************************************
 * @file LinkNodes.h
 * @brief 链表节点结构体定义
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.3.3
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DATA_STRUCTURE_LINK_NODES_H
#define DATA_STRUCTURE_LINK_NODES_H

/**
 * @struct DoubleLinkNode
 * @brief 双链表节点结构体
 * @tparam T 双链表存储的数据类型
 */
template<typename T>
struct DoubleLinkNode {
    DoubleLinkNode *prev{nullptr};
    DoubleLinkNode *next{nullptr};
    T data{};

    DoubleLinkNode() = default;

    explicit DoubleLinkNode(const T &src) : data(src) {}
};

#endif
