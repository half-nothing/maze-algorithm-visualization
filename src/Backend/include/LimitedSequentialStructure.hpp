/**********************************************
 * @file LimitedSequentialStructure.hpp
 * @brief 受限顺序结构的抽象父类
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license  GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_LIMITED_SEQUENTIAL_STRUCTURE_H
#define DAC_LIMITED_SEQUENTIAL_STRUCTURE_H

#include "Definition.h"

/**
 * @class LimitedSequentialStructure
 * @brief 受限顺序结构的抽象父类
 * @tparam T 受限顺序表存储的数据类型
 */
template<typename T>
class LimitedSequentialStructure {
    virtual ~LimitedSequentialStructure() = default;

    virtual void push(const T &src) = 0;

    virtual void push(InitList<T> src) = 0;

    virtual T &pop() = 0;

    virtual T &top() = 0;

    virtual bool isEmpty() = 0;

    virtual void clear() = 0;

    virtual uint size() = 0;
};

#endif
