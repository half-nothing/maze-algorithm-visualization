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

signals:
    void updateCostTime(QString);

public:
    static GraphPath *getInstance();

    void BFS(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end);

    void DFSStackVersion(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end);

    void DFSRecursiveVersion(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end);

    void GBFS(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end);

    void Dijkstra(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end);

    void aStar(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end);

private:
    static GraphPath *instance;
    bool returnFlag = false;

    GraphPath() = default;

    void updateTime(double time);

    void _DFSRecursiveVersion(std::vector<Point> &points, std::vector<Point> &path, QImage &image, QPoint start,
                              QPoint end, std::vector<std::vector<bool> > &vis);
};

#endif
