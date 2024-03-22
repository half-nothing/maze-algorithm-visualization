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

#include <ImageDisplay.h>

class GraphPath final : public QObject {
    Q_OBJECT

public:
    static GraphPath *getInstance();

    void bfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end);

    void dfs(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end);

    void aStar(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end);

private:
    static GraphPath *instance;

    GraphPath() = default;

    void _bfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end);

    void _dfs(std::vector<Point> &points, QImage &image, QPoint start, QPoint end,
              std::vector<std::vector<bool> > &vis, bool (*opt)(QRgb));

    void _aStar(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end);
};

#endif
