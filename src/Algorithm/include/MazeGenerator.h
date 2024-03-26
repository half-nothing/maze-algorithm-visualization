/**********************************************
 * @file MazeGenerator.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
#include <MazeMap.h>
#include <QObject>

class MazeGenerator final : public QObject {
    Q_OBJECT

signals:
    void updateCostTime(QString);

public:
    static MazeGenerator *getInstance();

    void DFS(MazeMap &mazeMap);

private:

    void updateTime(double time);
    static MazeGenerator *instance;

    MazeGenerator() = default;
};

#endif //MAZEGENERATOR_H
