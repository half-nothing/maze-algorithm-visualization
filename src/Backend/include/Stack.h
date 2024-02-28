#ifndef DAC_STACK_H
#define DAC_STACK_H

#include <initializer_list>
#include "Definition.h"


template<typename T>
class Stack {
    virtual void push(const T &src) = 0;

    virtual void push(const std::initializer_list<T> &src) = 0;

    virtual T &pop() = 0;

    virtual T &top() = 0;

    virtual bool isEmpty() = 0;

    virtual void clear() = 0;

    virtual uint size() = 0;
};


#endif
