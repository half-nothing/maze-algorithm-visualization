/**********************************************
 * @file SequentialStructure.hpp
 * @brief 顺序结构的抽象父类
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DATA_STRUCTURE_LIST_H
#define DATA_STRUCTURE_LIST_H

#include "Definition.h"

/**
 * @class SequentialStructure
 * @brief 顺序结构的抽象父类
 * @tparam T 顺序表存储的数据类型
 */
template<typename T>
class SequentialStructure {
public:
    virtual ~SequentialStructure() = default;

    virtual void clear() = 0;

    virtual bool isEmpty() = 0;

    virtual uint size() = 0;

    virtual void append(const T &src) = 0;

    virtual void append(InitList<T> src) = 0;

    virtual void insert(int pos, const T &src) = 0;

    virtual void insert(InitList<std::pair<int, T> > src) = 0;

    virtual void remove(int pos) = 0;

    virtual void remove(int start, int len) = 0;

    virtual T &get(int pos) = 0;

    virtual void setValue(int pos, const T &src) = 0;

    virtual int getPos(const T &src) = 0;

    virtual T &operator[](int pos) = 0;

    virtual void forEach(int start, int len, void (*opt)(T &)) = 0;
};

#endif
