/**********************************************
 * @file CircularlyLinkList.h
 * @brief 循环双链表头文件
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DATA_STRUCTURE_CIRCULARLY_LINK_LIST_H
#define DATA_STRUCTURE_CIRCULARLY_LINK_LIST_H

#include <iostream>
#include "LinkNodes.h"
#include "SequentialStructure.h"


/**
 * @brief 双循环链表
 * @tparam T 链表存储的数据类型
 */
template<typename T>
class CircularlyLinkList : public SequentialStructure<T> {
    using Node = DoubleLinkNode<T>;
    using NodePtr = DoubleLinkNode<T> *;
public:
    CircularlyLinkList() = default;

    CircularlyLinkList(InitList<T> init) {
        append(init);
    }

    /**
     * @see ::clear
     */
    ~CircularlyLinkList() {
        // 如果head和tail都没有指向,代表双链表为空或者出错,直接返回
        if (head == nullptr && tail == nullptr) {
            return;
        }
        NodePtr temp = head;
        NodePtr delPtr;
        uint total = length;
        while (length > 0) {
            delPtr = temp;
            temp = temp->next;
            delete delPtr;
            length--;
        }
        head = tail = nullptr;
        std::cout << "Release " << total - length << " nodes." << std::endl;
    }

    /**
     * @brief 清空循环双链表
     */
    void clear() override {
        if (head == nullptr && tail == nullptr) {
            return;
        }
        NodePtr temp = head;
        NodePtr delPtr;
        uint total = length;
        while (length > 0) {
            delPtr = temp;
            temp = temp->next;
            delete delPtr;
            length--;
        }
        length = 0;
        head = tail = nullptr;
        std::cout << "Release " << total - length << " nodes." << std::endl;
    }

    /**
     * @brief 判断双链表是否为空
     * @retval true 双链表是空的
     * @retval false 双链表非空
     */
    bool isEmpty() override {
        return length == 0;
    }

    /**
     * @brief 返回双链表的长度
     * @return 返回一个类型为unsigned int 类型的整数,表示双链表的长度
     */
    uint size() override {
        return length;
    }

    /**
     * @brief 向双链表尾部添加一个节点
     * @param[in] src 待插入节点的值
     * @par example:
     * @code
     *  CircularlyLinkList linkList{1, 2};
     *  linkList.append(3);
     * @endcode
     */
    void append(const T &src) override {
        auto temp = new Node(src);
        length++;
        if (head == nullptr && tail == nullptr) {
            head = tail = temp;
            return;
        }
        tail->next = temp;
        temp->prev = tail;
        head->prev = temp;
        temp->next = head;
    }

    /**
     * @brief 将一个列表依次插入双链表
     * @param[in] src 待插入的数字列表
     * @par example:
     * @code
     *  CircularlyLinkList linkList{1, 2};
     *  linkList.append({3, 4, 5});
     * @endcode
     */
    void append(InitList<T> src) override {
        for (const T &tmp: src) {
            append(tmp);
        }
    }

    /**
     * @brief 向双链表里面插入一个节点
     * @param[in] pos 节点的插入位置
     * @param[in] src 待插入节点的值
     * @par example:
     * @code
     *  CircularlyLinkList linkList{1, 2};
     *  linkList.insert(2, 5);
     * @endcode
     */
    void insert(uint pos, const T &src) override {
        NodePtr temp = findPtr(pos);
        auto tmp = new Node(src);
        tmp->next = temp;
        tmp->prev = temp->prev;
        temp->prev = tmp;
        tmp->prev->next = tmp;
        length++;
    }

    /**
     * @brief 将一个列表依次插入双链表的指定位置
     * @param[in] pos 待插入的数字列表
     * @par example:
     * @code
     *  CircularlyLinkList linkList{1, 2};
     *  linkList.insert({{2, 5}, {2, 6}});
     * @endcode
     */
    void insert(InitList<std::pair<uint, T>> src) override {
        for (const std::pair<uint, T> &tmp: src) {
            insert(tmp.first, tmp.second);
        }
    }

    void remove(uint pos) override {
        NodePtr temp = findPtr(pos);
        if (temp == head) {
            head = temp->next;
        }
        if (temp == tail) {
            tail = temp->prev;
        }
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
        length--;
    }

    void remove(uint start, uint len) override {
        if (start + len >= length) {
            if (start == 0) {
                clear();
                return;
            }
        }
    }

    T &get(uint pos) override {
        return findPtr(pos)->data;
    }

    void setValue(uint pos, const T &src) override {
        findPtr(pos)->data = src;
    }

    int getPos(const T &src) override {
        int index = 0;
        if (tail->data == src) {
            return length - 1;
        }
        NodePtr temp = head;
        while (temp != tail) {
            if (temp->data == src) return index;
            index++;
            temp = temp->next;
        }
        return -1;
    }

    T &operator[](int pos) override {
        return get(pos);
    }

    void forEach(uint start, uint len, void (*opt)(T &)) override {
    }

    friend std::ostream &operator<<(std::ostream &os, const CircularlyLinkList &list) {
        os << "CircularlyLinkList Length: " << list.length << std::endl
           << "Content: ";
        NodePtr temp = list.head;
        while (temp != list.tail) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << list.tail->data;
        return os;
    }

    void initIterator(uint pos, bool isReverse = false) {
        currentPtr = findPtr(pos);
        reverse = isReverse;
    }

    T &next() {
        if (currentPtr != nullptr) {
            T &temp = currentPtr->data;
            currentPtr = reverse ? currentPtr->prev : currentPtr->next;
            return temp;
        }
    }

private:
    NodePtr head = nullptr;         // 指向双链表的头结点
    NodePtr tail = nullptr;         // 指向双链表的尾节点
    uint length = 0;                // 表示双链表的长度
    bool reverse = false;           // 表明是否使用反向迭代器
    NodePtr currentPtr = nullptr;   // 迭代器当前指向的节点

    /**
     * @brief 通过给定的下标查找节点
     * @param[in] pos 想要查找的节点下标
     * @return 指向节点的指针
     */
    NodePtr findPtr(uint pos) {
        uint index = 0;
        NodePtr temp = head;
        while (temp != tail) {
            if (index == pos) return temp;
            index++;
            temp = temp->next;
        }
        return temp;
    }
};


#endif
