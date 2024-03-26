/**********************************************
 * @file MazeMap.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.27
 * @license GNU General Public License (GPL)
 **********************************************/

#include "MazeMap.h"
#include <QPainter>
#include <Config.h>

MazeMap::MazeMap(const int height, const int width):
    height{height}, width{width} {}

void MazeMap::setHeight(const int height) {
    this->height = height;
}

void MazeMap::setWidth(const int width) {
    this->width = width;
}

void MazeMap::setBorder() {
    hasBorder = true;
}

void MazeMap::removeBorder() {
    hasBorder = false;
}

void MazeMap::setWall(const int i, const int j) {
    map[i][j] = Wall;
}

void MazeMap::setRoad(const int i, const int j) {
    map[i][j] = Road;
}

bool MazeMap::isWall(const int i, const int j) const {
    return map[i][j] == Wall;
}

bool MazeMap::isRoad(const int i, const int j) const {
    return map[i][j] == Road;
}

bool MazeMap::isHasBorder() const {
    return hasBorder;
}

int MazeMap::getHeight() const {
    return height;
}

int MazeMap::getWidth() const {
    return width;
}

void MazeMap::linkRoad(const int x1, const int y1, const int x2, const int y2) {
    if (x1 == x2) {
        map[x1][(y1 + y2) / 2] = Road;
        return;
    }
    if (y1 == y2) {
        map[(x1 + x2) / 2][y1] = Road;
    }
}

bool MazeMap::judgePos(const int i, const int j) {
    if (hasBorder) {
        return i < 1 || i >= width - 1 || j < 1 || j >= height - 1;
    }
    return i < 0 || i >= width || j < 0 || j >= height;
}

void MazeMap::mapInit() {
    map = std::vector(height, std::vector(width, Wall));
    if (hasBorder) {
        for (int i = 1; i < height - 1; i += 2) {
            for (int j = 1; j < width - 1; j += 2) {
                map[i][j] = Road;
            }
        }
        return;
    }
    for (int i = 0; i < height; i += 2) {
        for (int j = 0; j < width; j += 2) {
            map[i][j] = Road;
        }
    }
}

QPixmap MazeMap::toQPixmap() const {
    QPixmap pixmap(width, height);
    pixmap.fill(Config::getInstance()->getConfigField(WALL_COLOR));
    QPainter painter;
    painter.begin(&pixmap);

    QPen pen;
    pen.setBrush(Qt::SolidPattern);
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.setBrush(Qt::SolidPattern);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j] == Road) {
                painter.drawPoint(i, j);
            }
        }
    }

    painter.end();
    return pixmap;
}
