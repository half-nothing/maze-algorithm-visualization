/**********************************************
 * @file Definition.h
 * @brief 宏定义以及类型别名定义
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.3.3
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_DEFINITION_H
#define DAC_DEFINITION_H

#include <QColor>
#include <QPoint>
#include <initializer_list>

#define STACK_MINIMUM_SIZE 8

template<class T>
using InitList = const std::initializer_list<T> &;

using exceotion = std::exception;

using uint = unsigned int;

struct Point {
    QPoint point;
    QColor color;

    friend bool operator==(const Point &lhs, const Point &rhs) {
        return lhs.point == rhs.point && lhs.color == rhs.color;
    }

    friend bool operator!=(const Point &lhs, const Point &rhs) {
        return !(lhs == rhs);
    }
};

#endif
