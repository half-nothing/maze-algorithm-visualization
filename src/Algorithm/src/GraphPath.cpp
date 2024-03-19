/**********************************************
 * @file GraphPath.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 24-3-3
 * @license GNU General Public License (GPL)
 **********************************************/

#include "GraphPath.h"

#include <glog/logging.h>

static bool returnFlag = false;
constexpr QRgb black = qRgb(0, 0, 0);

void GraphPath::bfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}

void GraphPath::dfs(QT::ImageDisplay *imageDisplay, const QPixmap &pixmap, const QPoint start, const QPoint end) {
    const clock_t timeStart = clock();
    std::vector vis(pixmap.width(), std::vector(pixmap.height(), false));
    QImage image = pixmap.toImage();
    returnFlag = false;
    _dfs(imageDisplay, image, start, end, vis, [](const QRgb color) {
        return color == black;
    });
    imageDisplay->addPoint({start, Qt::green});
    imageDisplay->addPoint({end, Qt::red});
    LOG(INFO) << "Dfs Time Consuming: "
            << 1000.0 * (clock() - timeStart) / CLOCKS_PER_SEC << "ms"
            << std::endl;
}

void GraphPath::aStar(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}

void GraphPath::_dfs(QT::ImageDisplay *imageDisplay, QImage &image, const QPoint start, const QPoint end,
                     std::vector<std::vector<bool> > &vis, bool (*opt)(QRgb)) {
    if (returnFlag) return;
    if (start.x() == end.x() && start.y() == end.y()) {
        returnFlag = true;
        return;
    }
    if (start.x() < 0 || start.x() >= image.width() || start.y() < 0 || start.y() >= image.height()) return;
    if (vis[start.x()][start.y()]) return;
    vis[start.x()][start.y()] = true;
    if (opt(image.pixel(start))) return;
    imageDisplay->addPoint({start, Qt::blue});

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            const auto temp = QPoint(start.x() + i, start.y() + j);
            _dfs(imageDisplay, image, temp, end, vis, opt);
        }
    }
}

void GraphPath::_bfs(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}
void GraphPath::_aStar(QPainter &painter, QPixmap &pixmap, QPoint start, QPoint end) {}
