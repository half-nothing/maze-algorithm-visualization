/**********************************************
 * @file SequentialStructure.h
 * @brief 顺序结构的抽象父类
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DATA_STRUCTURE_LIST_H
#define DATA_STRUCTURE_LIST_H

#include <initializer_list>
#include <utility>
#include "Definition.h"

template<typename T>
class SequentialStructure {
public:
    virtual void clear() = 0;

    virtual bool isEmpty() = 0;

    virtual uint size() = 0;

    virtual void append(const T &src) = 0;

    virtual void append(InitList<T> src) = 0;

    virtual void insert(uint pos, const T &src) = 0;

    virtual void insert(InitList<std::pair<uint, T>> src) = 0;

    virtual void remove(uint pos) = 0;

    virtual void remove(uint start, uint end) = 0;

    virtual T &get(uint pos) = 0;

    virtual void setValue(uint pos, const T &src) = 0;

    virtual int getPos(const T &src) = 0;

    virtual T &operator[](int pos) = 0;

    virtual void forEach(uint start, uint end, void (*opt)(T &)) = 0;
};

#endif
