/**********************************************
 * @file GraphPath.h
 * @brief 寻路算法头文件
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 24-3-3
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef GRAPHPATH_H
#define GRAPHPATH_H

#include <QPainter>

class GraphPath {
public:
    static void bfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end);

    static void dfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end);

    static void aStar(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end);
};

#endif
