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
#include <QThread>
#include <glog/logging.h>

static bool returnFlag = false;
constexpr QRgb black = qRgb(0, 0, 0);
constexpr int dirs[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};

GraphPath *GraphPath::instance = new GraphPath();

GraphPath *GraphPath::getInstance() {
    return instance;
}

void GraphPath::bfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}

void GraphPath::dfs(std::vector<Point> &points, const QPixmap &pixmap, const QPoint start, const QPoint end) {
    LARGE_INTEGER t1, t2, tc;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), false));
    std::vector<Point> path;
    QImage image = pixmap.toImage();
    returnFlag = false;
    _dfs(points, path, image, start, end, vis, [](const QRgb color) {
        return color == black;
    });
    points.insert(points.end(),
                  std::make_move_iterator(path.begin()),
                  std::make_move_iterator(path.end()));
    QueryPerformanceCounter(&t2);
    const double time = sc_double(t2.QuadPart - t1.QuadPart) / sc_double(tc.QuadPart);
    if (time < 1e-3) {
        LOG(INFO) << "Dfs Time Consuming: " << time * 1e6 << "us";
        emit updateCostTime(QString::asprintf("%.4lfus", time * 1e6));
        return;
    }
    if (time < 1) {
        LOG(INFO) << "Dfs Time Consuming: " << time * 1e3 << "ms";
        emit updateCostTime(QString::asprintf("%.4lfms", time * 1e3));
        return;
    }
    LOG(INFO) << "Dfs Time Consuming: " << time << "s";
    emit updateCostTime(QString::asprintf("%.4lfs", time));
}

void GraphPath::aStar(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}

void GraphPath::_dfs(std::vector<Point> &points, std::vector<Point> &path, QImage &image, const QPoint start,
                     const QPoint end,
                     std::vector<std::vector<bool> > &vis, bool (*opt)(QRgb)) {
    if (returnFlag) return;
    if (start.x() == end.x() && start.y() == end.y()) {
        path.emplace_back(start, Qt::yellow);
        returnFlag = true;
        return;
    }
    if (start.x() < 0 || start.x() >= image.width() || start.y() < 0 || start.y() >= image.height()) return;
    if (vis[start.x()][start.y()]) return;
    vis[start.x()][start.y()] = true;
    if (opt(image.pixel(start))) return;
    points.emplace_back(start, Qt::blue);

    for (const auto dir : dirs) {
        const auto temp = QPoint(start.x() + dir[0], start.y() + dir[1]);
        _dfs(points, path, image, temp, end, vis, opt);
        if (returnFlag) {
            path.emplace_back(temp, Qt::yellow);
            return;
        }
    }
}

void GraphPath::_bfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}
void GraphPath::_aStar(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}
