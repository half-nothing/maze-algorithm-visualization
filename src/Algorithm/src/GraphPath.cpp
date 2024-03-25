/**********************************************
 * @file GraphPath.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 24-3-3
 * @license GNU General Public License (GPL)
 **********************************************/

#include "windows.h"
#include "GraphPath.h"
#define  GLOG_NO_ABBREVIATED_SEVERITIES
#include <Config.h>
#include <QThread>
#include <Queue.hpp>
#include <Stack.hpp>
#include <TimeDefinition.h>
#include <glog/logging.h>

static constexpr int fourDirs[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
static constexpr int eightDirs[8][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
static constexpr QPoint invalidPoint(-1, -1);
static constexpr auto visitPoint = [](const QRgb color) {
    return color == Config::getInstance()->getConfigField(WALL_COLOR);
};

void GraphPath::updateTime(const double time) {
    if (time < 1e-3) {
        LOG(INFO) << "Time Consuming: " << time * 1e6 << "us";
        emit updateCostTime(QString::asprintf("%.4lfus", time * 1e6));
        return;
    }
    if (time < 1) {
        LOG(INFO) << "Time Consuming: " << time * 1e3 << "ms";
        emit updateCostTime(QString::asprintf("%.4lfms", time * 1e3));
        return;
    }
    LOG(INFO) << "Time Consuming: " << time << "s";
    emit updateCostTime(QString::asprintf("%.4lfs", time));
}

GraphPath *GraphPath::instance = new GraphPath();

GraphPath *GraphPath::getInstance() {
    return instance;
}

void GraphPath::bfs(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start, const QPoint end) {
    LOG(INFO) << "Bfs search start";
    const QImage image = pixmap.toImage();
    Queue<QPoint> queue;
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), QPoint(-1, -1)));
    TIMER_START
    if (visitPoint(image.pixel(start))) {
        goto EndFunc;
    }
    queue.push(start);
    vis[start.x()][start.y()] = start;
    while (!queue.isEmpty()) {
        const QPoint temp = queue.top();
        if (temp == end) {
            break;
        }
        for (const auto dir: fourDirs) {
            const auto tmp = QPoint(temp.x() + dir[0], temp.y() + dir[1]);
            if (tmp.x() < 0 || tmp.x() >= image.width() || tmp.y() < 0 || tmp.y() >= image.height()) continue;
            if (vis[tmp.x()][tmp.y()] == invalidPoint) {
                vis[tmp.x()][tmp.y()] = temp;
                if (visitPoint(image.pixel(tmp))) {
                    continue;
                }
                points.emplace_back(tmp, SEARCHED_POINT_COLOR);
                queue.push(tmp);
            }
        }
        queue.pop();
    }
    if (!queue.isEmpty() && queue.pop() == end) {
        QPoint tmp = end;
        while (!queue.isEmpty()) {
            const QPoint temp = vis[tmp.x()][tmp.y()];
            if (temp == start) {
                break;
            }
            points.emplace_back(temp, PATH_POINT_COLOR);
            tmp = temp;
        }
    }
EndFunc:
    TIMER_STOP
    LOG(INFO) << "Bfs search finish";
    updateTime(time);
}

void GraphPath::dfsStackVersion(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start,
                                const QPoint end) {
    LOG(INFO) << "DfsStackVersion search start";
    const QImage image = pixmap.toImage();
    Stack<QPoint> stack;
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), false));
    TIMER_START
    if (visitPoint(image.pixel(start))) {
        goto EndFunc;
    }
    stack.push(start);
    while (!stack.isEmpty()) {
        const QPoint temp = stack.top();
        if (temp == end) {
            break;
        }
        for (const auto dir: fourDirs) {
            const auto tmp = QPoint(temp.x() + dir[0], temp.y() + dir[1]);
            if (tmp.x() < 0 || tmp.x() >= image.width() || tmp.y() < 0 || tmp.y() >= image.height()) continue;
            if (vis[tmp.x()][tmp.y()] == false) {
                vis[tmp.x()][tmp.y()] = true;
                if (visitPoint(image.pixel(tmp))) {
                    continue;
                }
                points.emplace_back(tmp, SEARCHED_POINT_COLOR);
                stack.push(tmp);
                goto EndLoop;
            }
        }
        stack.pop();
    EndLoop:
        continue;
    }
    if (!stack.isEmpty() && stack.pop() == end) {
        while (!stack.isEmpty()) {
            points.emplace_back(stack.pop(), PATH_POINT_COLOR);
        }
    }
EndFunc:
    TIMER_STOP
    LOG(INFO) << "DfsStackVersion search finish";
    updateTime(time);
}

void GraphPath::dfsRecursiveVersion(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start,
                                    const QPoint end) {
    LOG(INFO) << "DfsRecursiveVersion search start";
    TIMER_START
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), false));
    std::vector<Point> path;
    QImage image = pixmap.toImage();
    returnFlag = false;
    _dfsRecursiveVersion(points, path, image, start, end, vis);
    points.insert(points.end(),
                  std::make_move_iterator(path.begin()),
                  std::make_move_iterator(path.end()));
    TIMER_STOP
    LOG(INFO) << "DfsRecursiveVersion search finish";
    updateTime(time);
}

void GraphPath::_dfsRecursiveVersion(std::vector<Point> &points, std::vector<Point> &path, QImage &image,
                                     const QPoint start,
                                     const QPoint end,
                                     std::vector<std::vector<bool> > &vis) {
    if (returnFlag) return;
    if (start.x() == end.x() && start.y() == end.y()) {
        returnFlag = true;
        return;
    }
    if (start.x() < 0 || start.x() >= image.width() || start.y() < 0 || start.y() >= image.height()) return;
    if (vis[start.x()][start.y()]) return;
    vis[start.x()][start.y()] = true;
    if (visitPoint(image.pixel(start))) return;
    points.emplace_back(start, SEARCHED_POINT_COLOR);

    for (const auto dir: fourDirs) {
        const auto temp = QPoint(start.x() + dir[0], start.y() + dir[1]);
        _dfsRecursiveVersion(points, path, image, temp, end, vis);
        if (returnFlag) {
            path.emplace_back(temp, PATH_POINT_COLOR);
            return;
        }
    }
}

void GraphPath::GBFS(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end) {}
void GraphPath::Dijkstra(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end) {}
void GraphPath::aStar(std::vector<Point> &points, const QPixmap &pixmap, QPoint start, QPoint end) {}
