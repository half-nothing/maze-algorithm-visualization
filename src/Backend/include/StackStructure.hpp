/**********************************************
 * @file StackStructure.hpp
 * @brief 受限顺序结构的抽象父类
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license  GNU General Public License (GPL)
 **********************************************/
#ifndef STACK_STRUCTURE_H
#define STACK_STRUCTURE_H

#include "Definition.h"

/**
 * @class StackStructure
 * @brief 栈结构的抽象父类
 * @tparam T 栈存储的数据类型
 */
template<typename T>
class StackStructure {
public:
    virtual ~StackStructure() = default;

    virtual void push(const T &src) = 0;

    virtual void push(InitList<T> src) = 0;

    virtual T &pop() = 0;

    virtual T &top() = 0;

    virtual bool isEmpty() = 0;

    virtual void clear() = 0;

    virtual uint size() = 0;
};

#endif
