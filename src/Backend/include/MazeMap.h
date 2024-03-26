/**********************************************
 * @file MazeMap.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.27
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef MAZEMAP_H
#define MAZEMAP_H
#include <QPixmap>
#include <vector>

class MazeMap {
public:
    MazeMap(int height, int width);

    void setHeight(int height);

    void setWidth(int width);

    void setBorder();

    void removeBorder();

    void setWall(int i, int j);

    void setRoad(int i, int j);

    bool isWall(int i, int j) const;

    bool isRoad(int i, int j) const;

    bool isHasBorder() const;

    int getHeight() const;

    int getWidth() const;

    void linkRoad(int x1, int y1, int x2, int y2);

    bool judgePos(int i, int j);

    void mapInit();

    QPixmap toQPixmap() const;

private:
    enum Type {
        Road,
        Wall
    };

    std::vector<std::vector<Type> > map;
    int height;
    int width;
    bool hasBorder = true;
};

#endif
