/**********************************************
 * @file PriorityQueue.hpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.04.09
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP
#include <Queue.hpp>

template<typename T, typename compare = std::less<T>>
class PriorityQueue final : public Queue<T> {
    using Node = DoubleLinkNode<T>;
    using NodePtr = DoubleLinkNode<T> *;
public:
    void push(const T &src) override {
        Queue<T>::push(src);
        NodePtr temp = this->head->next;
        NodePtr small = this->head;
        while (temp != this->tail) {
            if (comp(small->data, temp->data)) {
                small = temp;
            }
            temp = temp->next;
        }
        if (comp(small->data, this->tail->data)) {
            this->head = this->tail;
            this->tail = this->tail->prev;
            return;
        }
        if (small == this->head) {
            return;
        }
        small->next->prev = small->prev;
        small->prev->next = small->next;
        small->next = this->head;
        small->prev = this->tail;
        this->tail->next = small;
        this->head->prev = small;
        this->head = small;
    }
private:
    compare comp{};
};

#endif //PRIORITYQUEUE_HPP
