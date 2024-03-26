/**********************************************
 * @file GraphPath.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 24-3-3
 * @license GNU General Public License (GPL)
 **********************************************/

#include <queue>
#include "windows.h"
#include "GraphPath.h"
#define  GLOG_NO_ABBREVIATED_SEVERITIES
#include <Config.h>
#include <QThread>
#include <Queue.hpp>
#include <Stack.hpp>
#include <TimeDefinition.h>
#include <complex>
#include <glog/logging.h>

static constexpr int fourDirs[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
static constexpr int eightDirs[8][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
static constexpr QPoint invalidPoint(-1, -1);
static constexpr auto visitPoint = [](const QRgb color) {
    return color == Config::getInstance()->getConfigField(WALL_COLOR);
};
static constexpr auto getManhattanDistance = [](const QPoint &start, const QPoint &end) {
    return sc_double(abs(start.x() - end.x()) + std::abs(start.y() - end.y()));
};
static constexpr auto getEuclideanDistance = [](const QPoint &start, const QPoint &end) {
    return sqrt(std::pow(start.x() - end.x(), 2) + std::pow(start.y() - end.y(), 2));
};

struct StorePoint {
    QPoint point;
    double distance;

    StorePoint():
        point{invalidPoint}, distance{-1} {};

    StorePoint(const QPoint &point, const double distance) :
        point{point},
        distance{distance} {}

    StorePoint(const StorePoint &other) = default;

    StorePoint(StorePoint &&other) noexcept :
        point{other.point},
        distance{other.distance} {}

    StorePoint &operator=(const StorePoint &other) {
        if (this == &other) return *this;
        point = other.point;
        distance = other.distance;
        return *this;
    }

    StorePoint &operator=(StorePoint &&other) noexcept {
        if (this == &other) return *this;
        point = other.point;
        distance = other.distance;
        return *this;
    }
};

struct AStarPoint final : StorePoint {
    double weight;

    AStarPoint():
        weight{1e9} {}

    AStarPoint(const QPoint &point, const double distance, const double weight) :
        StorePoint{point, distance},
        weight{weight} {}

    AStarPoint(const AStarPoint &other) = default;

    AStarPoint(AStarPoint &&other) noexcept :
        StorePoint{std::move(other)},
        weight{other.weight} {}

    AStarPoint &operator=(const AStarPoint &other) {
        if (this == &other) return *this;
        StorePoint::operator =(other);
        weight = other.weight;
        return *this;
    }

    AStarPoint &operator=(AStarPoint &&other) noexcept {
        if (this == &other) return *this;
        StorePoint::operator =(std::move(other));
        weight = other.weight;
        return *this;
    }
};

struct StorePointCmp {
    bool operator()(const StorePoint &first, const StorePoint &second) const {
        return first.distance > second.distance;
    }
};

struct AStarPointCmp {
    bool operator()(const AStarPoint &first, const AStarPoint &second) const {
        return first.weight > second.weight;
    }
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

void GraphPath::BFS(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start, const QPoint end) {
    LOG(INFO) << "Bfs search start";
    const QImage image = pixmap.toImage();
    Queue<QPoint> queue;
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), invalidPoint));
    TIMER_START
    if (visitPoint(image.pixel(start))) {
        goto EndFunc;
    }
    queue.push(start);
    vis[start.x()][start.y()] = start;
    while (!queue.isEmpty()) {
        const QPoint temp = queue.top();
        points.emplace_back(temp, SEARCHED_POINT_COLOR);
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
                points.emplace_back(tmp, SEARCHING_POINT_COLOR);
                queue.push(tmp);
            }
        }
        queue.pop();
    }
    if (vis[end.x()][end.y()] != invalidPoint) {
        QPoint tmp = end;
        while (true) {
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

void GraphPath::DFSStackVersion(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start,
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

void GraphPath::DFSRecursiveVersion(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start,
                                    const QPoint end) {
    LOG(INFO) << "DfsRecursiveVersion search start";
    TIMER_START
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), false));
    std::vector<Point> path;
    QImage image = pixmap.toImage();
    returnFlag = false;
    _DFSRecursiveVersion(points, path, image, start, end, vis);
    points.insert(points.end(),
                  std::make_move_iterator(path.begin()),
                  std::make_move_iterator(path.end()));
    TIMER_STOP
    LOG(INFO) << "DfsRecursiveVersion search finish";
    updateTime(time);
}

void GraphPath::_DFSRecursiveVersion(std::vector<Point> &points, std::vector<Point> &path, QImage &image,
                                     const QPoint start, const QPoint end,
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
        _DFSRecursiveVersion(points, path, image, temp, end, vis);
        if (returnFlag) {
            path.emplace_back(temp, PATH_POINT_COLOR);
            return;
        }
    }
}

void GraphPath::GBFS(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start,
                     const QPoint end, const bool useManhattan) {
    LOG(INFO) << "GBFS search start";
    const QImage image = pixmap.toImage();
    double (*opt)(const QPoint &, const QPoint &) = getEuclideanDistance;
    if (useManhattan) {
        opt = getManhattanDistance;
    }
    std::priority_queue<StorePoint, std::vector<StorePoint>, StorePointCmp> queue;
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), invalidPoint));
    TIMER_START
    if (visitPoint(image.pixel(start))) {
        goto EndFunc;
    }
    queue.emplace(start, opt(start, end));
    while (!queue.empty()) {
        const QPoint temp = queue.top().point;
        queue.pop();
        points.emplace_back(temp, SEARCHED_POINT_COLOR);
        if (temp == end) {
            break;
        }
        for (const auto dir: eightDirs) {
            const auto tmp = QPoint(temp.x() + dir[0], temp.y() + dir[1]);
            if (tmp.x() < 0 || tmp.x() >= image.width() || tmp.y() < 0 || tmp.y() >= image.height()) continue;
            if (vis[tmp.x()][tmp.y()] == invalidPoint) {
                vis[tmp.x()][tmp.y()] = temp;
                if (visitPoint(image.pixel(tmp))) {
                    continue;
                }
                points.emplace_back(tmp, SEARCHING_POINT_COLOR);
                queue.emplace(tmp, opt(tmp, end));
            }
        }
    }
    if (vis[end.x()][end.y()] != invalidPoint) {
        QPoint tmp = end;
        while (true) {
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
    LOG(INFO) << "GBFS search finish";
    updateTime(time);
}

void GraphPath::Dijkstra(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start,
                         const QPoint end, const bool useManhattan) {
    LOG(INFO) << "Dijkstra search start";
    const QImage image = pixmap.toImage();
    double (*opt)(const QPoint &, const QPoint &) = getEuclideanDistance;
    if (useManhattan) {
        opt = getManhattanDistance;
    }
    std::priority_queue<StorePoint, std::vector<StorePoint>, StorePointCmp> queue;
    std::vector vis(pixmap.width(), std::vector<StorePoint>(pixmap.height()));
    std::vector startVis(pixmap.width(), std::vector(pixmap.height(), false));
    TIMER_START
    if (visitPoint(image.pixel(start))) {
        goto EndFunc;
    }
    vis[start.x()][start.y()].distance = 0;
    vis[start.x()][start.y()].point = start;
    queue.emplace(start, vis[start.x()][start.y()].distance);
    while (!queue.empty()) {
        const QPoint temp = queue.top().point;
        queue.pop();
        if (startVis[temp.x()][temp.y()]) {
            continue;
        }
        startVis[temp.x()][temp.y()] = true;
        points.emplace_back(temp, SEARCHED_POINT_COLOR);
        if (temp == end) {
            break;
        }
        for (const auto dir: eightDirs) {
            const auto tmp = QPoint(temp.x() + dir[0], temp.y() + dir[1]);
            if (tmp.x() < 0 || tmp.x() >= image.width() || tmp.y() < 0 || tmp.y() >= image.height()) continue;
            if (!visitPoint(image.pixel(tmp)) &&
                (vis[tmp.x()][tmp.y()].point == invalidPoint ||
                 vis[tmp.x()][tmp.y()].distance > vis[temp.x()][temp.y()].distance + opt(temp, tmp))) {
                vis[tmp.x()][tmp.y()].point = temp;
                vis[tmp.x()][tmp.y()].distance = vis[temp.x()][temp.y()].distance + opt(temp, tmp);
                points.emplace_back(tmp, SEARCHING_POINT_COLOR);
                queue.emplace(tmp, vis[tmp.x()][tmp.y()].distance);
            }
        }
    }
    if (vis[end.x()][end.y()].point != invalidPoint) {
        QPoint tmp = end;
        while (true) {
            const QPoint temp = vis[tmp.x()][tmp.y()].point;
            if (temp == start) {
                break;
            }
            points.emplace_back(temp, PATH_POINT_COLOR);
            tmp = temp;
        }
    }
EndFunc:
    TIMER_STOP
    LOG(INFO) << "Dijkstra search finish";
    updateTime(time);
}

void GraphPath::aStar(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start, const QPoint end,
                      const bool useManhattan) {
    LOG(INFO) << "A* search start";
    const QImage image = pixmap.toImage();
    double (*opt)(const QPoint &, const QPoint &) = getEuclideanDistance;
    if (useManhattan) {
        opt = getManhattanDistance;
    }
    std::priority_queue<AStarPoint, std::vector<AStarPoint>, AStarPointCmp> queue;
    std::vector vis(pixmap.width(), std::vector<AStarPoint>(pixmap.height()));
    std::vector startVis(pixmap.width(), std::vector(pixmap.height(), false));
    TIMER_START
    if (visitPoint(image.pixel(start))) {
        goto EndFunc;
    }
    vis[start.x()][start.y()].distance = 0;
    vis[start.x()][start.y()].weight = 0;
    vis[start.x()][start.y()].point = start;
    queue.emplace(start, vis[start.x()][start.y()].distance, vis[start.x()][start.y()].weight);
    while (!queue.empty()) {
        const QPoint temp = queue.top().point;
        queue.pop();
        if (startVis[temp.x()][temp.y()]) {
            continue;
        }
        startVis[temp.x()][temp.y()] = true;
        points.emplace_back(temp, SEARCHED_POINT_COLOR);
        if (temp == end) {
            break;
        }
        for (const auto dir: eightDirs) {
            const auto tmp = QPoint(temp.x() + dir[0], temp.y() + dir[1]);
            if (tmp.x() < 0 || tmp.x() >= image.width() || tmp.y() < 0 || tmp.y() >= image.height()) continue;
            if (!visitPoint(image.pixel(tmp)) &&
                (vis[tmp.x()][tmp.y()].point == invalidPoint ||
                 vis[tmp.x()][tmp.y()].distance > vis[temp.x()][temp.y()].distance + opt(temp, tmp))) {
                vis[tmp.x()][tmp.y()].point = temp;
                vis[tmp.x()][tmp.y()].distance = vis[temp.x()][temp.y()].distance + opt(temp, tmp);
                vis[tmp.x()][tmp.y()].weight = vis[tmp.x()][tmp.y()].distance + opt(tmp, end);
                points.emplace_back(tmp, SEARCHING_POINT_COLOR);
                queue.emplace(tmp, vis[tmp.x()][tmp.y()].distance, vis[tmp.x()][tmp.y()].weight);
            }
        }
    }
    if (vis[end.x()][end.y()].point != invalidPoint) {
        QPoint tmp = end;
        while (true) {
            const QPoint temp = vis[tmp.x()][tmp.y()].point;
            if (temp == start) {
                break;
            }
            points.emplace_back(temp, PATH_POINT_COLOR);
            tmp = temp;
        }
    }
EndFunc:
    TIMER_STOP
    LOG(INFO) << "A* search finish";
    updateTime(time);
}
