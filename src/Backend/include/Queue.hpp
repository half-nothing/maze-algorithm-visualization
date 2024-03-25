/**********************************************
 * @file Queue.hpp
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
#include "QueueStructure.hpp"
#include "glog/logging.h"

/**
 * @class Queue
 * @brief 双循环链表
 * @tparam T 链表存储的数据类型
 */
template<typename T>
class Queue final : public QueueStructure<T> {
    using Node = DoubleLinkNode<T>;
    using NodePtr = DoubleLinkNode<T> *;

public:
    Queue() = default;

    Queue(InitList<T> init) {
        append(init);
    }

    ~Queue() override {
        if (head == nullptr && tail == nullptr) {
            return;
        }
        NodePtr temp = head;
        NodePtr delPtr = temp;
        while (length > 0) {
            delPtr = temp;
            temp = temp->next;
            delete delPtr;
            length--;
        }
        head = tail = nullptr;
    }

    void clear() override {
        if (head == nullptr && tail == nullptr) {
            return;
        }
        NodePtr temp = head;
        NodePtr delPtr = temp;
        while (length > 0) {
            delPtr = temp;
            temp = temp->next;
            delete delPtr;
            length--;
        }
        length = 0;
        head = tail = nullptr;
    }

    T &pop() override {
        if (length == 0 || (head == nullptr && tail == nullptr)) {
            throw exception();
        }
        if (length == 1) {
            T &data = head->data;
            delete head;
            head = tail = nullptr;
            length--;
            return data;
        }
        NodePtr temp = head;
        head = head->next;
        head->prev = tail;
        tail->next = head;
        T &data = temp->data;
        delete temp;
        length--;
        return data;
    }

    T &top() override {
        return head->data;
    }

    bool isEmpty() override {
        return length == 0;
    }

    uint size() override {
        return length;
    }

    void push(const T &src) override {
        auto temp = new Node(src);
        length++;
        if (head == nullptr && tail == nullptr) {
            temp->next = temp;
            temp->prev = temp;
            head = tail = temp;
            return;
        }
        temp->prev = tail;
        temp->next = head;
        head->prev = temp;
        tail->next = temp;
        tail = temp;
    }

    void push(InitList<T> src) override{
        for (const T &tmp: src) {
            push(tmp);
        }
    }

private:
    NodePtr head = nullptr;
    NodePtr tail = nullptr;
    uint length = 0;

    NodePtr findPtr(const int pos) {
        if (pos < 0 || pos >= length) throw exception();
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
