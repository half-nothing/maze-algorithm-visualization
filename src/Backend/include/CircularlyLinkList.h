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
#include "glog/logging.h"

/**
 * @class CircularlyLinkList
 * @brief 双循环链表
 * @tparam T 链表存储的数据类型
 */
template<typename T>
class CircularlyLinkList final : public SequentialStructure<T> {
    using Node = DoubleLinkNode<T>;
    using NodePtr = DoubleLinkNode<T> *;

public:
    CircularlyLinkList() = default;

    CircularlyLinkList(InitList<T> init) {
        append(init);
    }

    /**
     * @link CircularlyLinkList::clear
     */
    ~CircularlyLinkList() override {
        // 如果head和tail都没有指向,代表双链表为空或者出错,直接返回
        if (head == nullptr && tail == nullptr) {
            return;
        }
        NodePtr temp = head;
        NodePtr delPtr = temp;
        const uint total = length;
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
        NodePtr delPtr = temp;
        const uint total = length;
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
     * @return true 双链表是空的
     * @return false 双链表非空
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
    void insert(const int pos, const T &src) override {
        if (pos < 0) {
            LOG(ERROR) << "The index must be positive, but got " << pos << std::endl;
            return;
        }
        assert(pos >= 0);
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
     * @param[in] src 待插入的数字列表
     * @par example:
     * @code
     *  CircularlyLinkList linkList{1, 2};
     *  linkList.insert({{2, 5}, {2, 6}});
     * @endcode
     */
    void insert(InitList<std::pair<int, T> > src) override {
        for (const std::pair<uint, T> &tmp: src) {
            insert(tmp.first, tmp.second);
        }
    }

    /**
     * @brief 移除指定下标的节点
     * @param[in] pos 移除节点的下标
     */
    void remove(const int pos) override {
        if (pos < 0 || pos >= length) return;
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

    /**
     * @brief 移除从指定节点开始,len个节点
     * @param[in] start 开始的节点下标
     * @param[in] len 要删除的长度
     */
    void remove(const int start, const int len) override {
        if (start < 0 || len <= 0 || start > length - 1) return;
        if (len == 1) {
            remove(start);
            return;
        }
        NodePtr temp;
        if (len >= length || start + len >= length) {
            if (start == 0) {
                clear();
                return;
            }
            NodePtr startPtr = findPtr(start);
            startPtr->prev->next = tail->next;
            tail->next->prev = startPtr->prev;
            while (startPtr != tail) {
                temp = startPtr->next;
                delete startPtr;
                startPtr = temp;
            }
            delete startPtr;
            tail = head->prev;
            length = start;
            return;
        }
        const uint end = start + len - 1;
        NodePtr startPtr = findPtr(start);
        NodePtr endPtr = findPtr(end);
        startPtr->prev->next = endPtr->next;
        endPtr->next->prev = startPtr->prev;
        while (startPtr != endPtr) {
            temp = startPtr->next;
            delete startPtr;
            startPtr = temp;
        }
        if (start == 0) { head = startPtr->next; }
        delete startPtr;
        length -= len;
    }

    /**
     * @brief 获取指定下标的节点存储的数据
     * @param[in] pos 指定节点的下标
     * @return 返回存储数据的引用
     */
    T &get(const int pos) override {
        if (pos < 0 || pos >= length) throw exceotion();
        assert(0 <= pos && pos < length);
        return findPtr(pos)->data;
    }

    /**
     * @brief 设置指定下标的节点的数据
     * @param[in] pos 指定节点的下标
     * @param[in] src 要修改成的数据
     */
    void setValue(const int pos, const T &src) override {
        if (pos < 0 || pos >= length) return;
        assert(0 <= pos && pos < length);
        findPtr(pos)->data = src;
    }

    /**
     * @brief 获取包含指定数据的节点的下标
     * @param[in] src 指定包含的数据
     * @retval >0 包含指定数据的节点下标
     * @retval -1 未找到包含指定数据的节点下标
     */
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

    /**
     * @brief 对[]运算符的重载
     */
    T &operator[](const int pos) override {
        if (pos < 0 || pos >= length) throw exceotion();
        assert(0 <= pos && pos < length);
        return get(pos);
    }

    /**
     * @brief 对指定范围内的节点执行某种操作
     * @param[in] start 开始节点的下标
     * @param[in] len 指定的长度
     * @param[in] opt 要进行的操作
     */
    void forEach(int start, const int len, void (*opt)(T &)) override {
        if (start >= length || len == 0) return;
        if (len == 1) {
            opt(findPtr(start)->data);
            return;
        }
        if (start < 0) {
            start = 0;
        }
        NodePtr startPtr = findPtr(start);
        const int end = start + len;
        if (end >= length) {
            while (startPtr != tail) {
                opt(startPtr->data);
                startPtr = startPtr->next;
            }
            opt(startPtr->data);
            return;
        }
        NodePtr endPtr = findPtr(end);
        while (startPtr != endPtr) {
            opt(startPtr->data);
            startPtr = startPtr->next;
        }
    }

    /**
     * @brief 重载对cout的运算符实现打印输出
     */
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

    /**
     * @brief 初始化迭代器
     * @param[in] pos 迭代器的开始坐标
     * @param[in] isReverse 是否为反向迭代器
     */
    void initIterator(const int pos, const bool isReverse = false) {
        if (pos < 0 || pos >= length) return;
        assert(0 <= pos && pos < length);
        currentPtr = findPtr(pos);
        reverse = isReverse;
    }

    /**
     * @brief 获取迭代器的下一个数据
     * @param[out] val 接受数据的变量引用
     * @pre 首先需要调用::initIterator初始化迭代器
     * @note 禁止在调用::initIterator之前调用本函数
     * @return 返回存储数据的引用
     */
    bool next(T &val) {
        if (currentPtr != nullptr) {
            val = currentPtr->data;
            currentPtr = reverse ? currentPtr->prev : currentPtr->next;
            return true;
        }
        return false;
    }

private:
    NodePtr head = nullptr;       // 指向双链表的头结点
    NodePtr tail = nullptr;       // 指向双链表的尾节点
    uint length = 0;              // 表示双链表的长度
    bool reverse = false;         // 表明是否使用反向迭代器
    NodePtr currentPtr = nullptr; // 迭代器当前指向的节点

    /**
     * @brief 通过给定的下标查找节点
     * @param[in] pos 想要查找的节点下标
     * @return 指向节点的指针
     */
    NodePtr findPtr(const int pos) {
        if (pos < 0 || pos >= length) throw exceotion();
        assert(0 <= pos && pos < length);
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
