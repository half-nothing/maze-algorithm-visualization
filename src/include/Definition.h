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

#define sc_int(x) static_cast<int>(x)
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

    Point(const QPoint &point, const QColor &color) :
        point{point},
        color{color} {}

    Point(const Point &other) = default;

    Point(Point &&other) noexcept :
        point(other.point),
        color(other.color) {}

    Point &operator=(const Point &other) {
        if (this == &other) return *this;
        point = other.point;
        color = other.color;
        return *this;
    }

    Point &operator=(Point &&other) noexcept {
        if (this == &other) return *this;
        point = other.point;
        color = other.color;
        return *this;
    }
};

enum BmpType {
    UNKNOWN_BIT,
    ONE_BIT,
    EIGHT_BIT,
    TWENTY_FOUR_BIT
};

#endif
