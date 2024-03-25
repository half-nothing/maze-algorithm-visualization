/**********************************************
 * @file QueueStructure.hpp
 * @brief 顺序结构的抽象父类
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef QUEUE_STRUCTURE_H
#define QUEUE_STRUCTURE_H

#include "Definition.h"

/**
 * @class QueueStructure
 * @brief 队列结构的抽象父类
 * @tparam T 队列存储的数据类型
 */
template<typename T>
class QueueStructure {
public:
    virtual ~QueueStructure() = default;

    virtual void push(const T &src) = 0;

    virtual void push(InitList<T> src) = 0;

    virtual T &pop() = 0;

    virtual T &top() = 0;

    virtual bool isEmpty() = 0;

    virtual void clear() = 0;

    virtual uint size() = 0;
};

#endif
