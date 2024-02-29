#ifndef DAC_DEFINITION_H
#define DAC_DEFINITION_H

#include <initializer_list>

#define STACK_MINIMUM_SIZE 8

template<class T>
using InitList = const std::initializer_list<T> &;

typedef unsigned int uint;

#endif
