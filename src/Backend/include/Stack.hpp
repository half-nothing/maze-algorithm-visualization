/**********************************************
 * @file Stack.hpp
 * @brief 顺序栈头文件
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_SEQUENTIAL_STACK_H
#define DAC_SEQUENTIAL_STACK_H

#include <exception>
#include <iostream>
#include "LimitedSequentialStructure.hpp"

/**
 * @class Stack
 * @brief 顺序栈
 * @tparam T 存储的数据类型
 */
template<typename T>
class Stack final : public LimitedSequentialStructure<T> {
public:
    Stack():
        Stack(STACK_MINIMUM_SIZE) {}

    explicit Stack(const int capacity) :
        capacity(capacity) {
        data = new T[capacity];
    }

    Stack(const T &src, const int capacity) :
        Stack(capacity) {
        std::fill(data, data + capacity, src);
    }

    Stack(InitList<T> init) :
        Stack() {
        push(init);
    }

    ~Stack() override {
        delete[] data;
    }

    /**
     * @brief 向栈内推入一个元素
     * @param[in] src 要推入元素的引用
     */
    void push(const T &src) override {
        if (now < capacity - 1) {
            data[++now] = src;
            return;
        }
        resize(capacity * 2);
        data[++now] = src;
    }

    /**
     * @brief 向栈内推入一个列表
     * @param[in] src 要推入的列表
     */
    void push(InitList<T> src) override {
        for (const T &tmp: src) {
            push(tmp);
        }
    }

    /**
     * @brief 从栈内退出一个元素
     * @return 栈顶的元素引用
     */
    T &pop() override {
        if (now == -1) throw std::exception();
        T &temp = data[now--];
        if (now <= capacity / 4 && capacity / 2 >= STACK_MINIMUM_SIZE) {
            resize(capacity / 2);
        }
        return temp;
    }

    /**
     * @brief 从栈内取出一个元素
     * @return 栈顶的元素引用
     */
    T &top() override {
        if (now == -1) throw std::exception();
        return data[now];
    }

    /**
     * @brief 判断栈是否为空
     * @return true 栈是空栈
     * @return false 栈非空
     */
    bool isEmpty() override { return now == -1; }

    /**
     * @brief 清空栈
     */
    void clear() override {
        delete[] data;
        now  = -1;
        data = new T[capacity];
    }

    /**
     * @brief 返回栈内存储的元素
     * @return 栈存储的元素个数
     */
    uint size() override {
        return now + 1;
    }

    /**
     * @brief 重载对cout的运算符实现打印输出
     */
    friend std::ostream &operator<<(std::ostream &os, const Stack &stack) {
        os << "Stack Capacity Usage: " << stack.now + 1 << "/" << stack.capacity << std::endl
                << "Content: ";
        for (int i = 0; i < stack.now + 1; ++i) { std::cout << stack.data[i] << " "; }
        return os;
    }

private:
    T *data      = nullptr;
    int now      = -1;
    int capacity = 0;

    /**
     * @brief 重新分配栈存储空间
     * @param[in] newSize 重新分配后的存储空间大小
     */
    void resize(const int newSize) {
        capacity = newSize;
        T *temp  = data;
        data     = new T[capacity];
        for (int i = 0; i < now + 1; ++i) { data[i] = temp[i]; }
        delete[] temp;
    }
};

#endif
