/**********************************************
 * @file MazeGenerator.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "MazeGenerator.h"

#include <Stack.hpp>
#include <TimeDefinition.h>
#include <random>
#include <set>
#include <glog/logging.h>

static constexpr int fourDirs[4][2] = {{2, 0}, {0, -2}, {-2, 0}, {0, 2}};

MazeGenerator *MazeGenerator::instance = new MazeGenerator();

MazeGenerator *MazeGenerator::getInstance() {
    return instance;
}

void MazeGenerator::updateTime(const double time) {
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

void MazeGenerator::DFS(MazeMap &mazeMap) {
    LOG(INFO) << "Dfs generate start";
    const int height = mazeMap.getHeight();
    const int width = mazeMap.getWidth();
    std::mt19937 gen{std::random_device()()};
    std::uniform_int_distribution distrib(0, 3);
    std::vector visitDirs(height, std::vector<std::set<int> >(width));
    std::vector vis(height, std::vector(width, false));
    Stack<QPoint> stack;
    TIMER_START
    if (mazeMap.isHasBorder()) {
        stack.push({1, 1});
        vis[1][1] = true;
    } else {
        stack.push({0, 0});
        vis[0][0] = true;
    }
    while (!stack.isEmpty()) {
        const QPoint temp = stack.top();
        if (visitDirs[temp.x()][temp.y()].size() == 4) {
            stack.pop();
            continue;
        }
        int dir;
        while (true) {
            dir = distrib(gen);
            if (!visitDirs[temp.x()][temp.y()].contains(dir)) {
                visitDirs[temp.x()][temp.y()].insert(dir);
                break;
            }
        }
        const auto tmp = QPoint(temp.x() + fourDirs[dir][0], temp.y() + fourDirs[dir][1]);
        if (mazeMap.judgePos(tmp.x(), tmp.y())) continue;
        if (vis[tmp.x()][tmp.y()]) continue;
        vis[tmp.x()][tmp.y()] = true;
        mazeMap.linkRoad(temp.x(), temp.y(), tmp.x(), tmp.y());
        stack.push(tmp);
    }
    TIMER_STOP
    LOG(INFO) << "Dfs generate finish";
    updateTime(time);
}

void MazeGenerator::Prim(MazeMap &mazeMap) {
    LOG(INFO) << "Prim generate start";
    using stdParam = std::uniform_int_distribution<>::param_type;
    const int height = mazeMap.getHeight();
    const int width = mazeMap.getWidth();
    std::mt19937 gen{std::random_device()()};
    std::uniform_int_distribution distrib;
    std::vector vis(height, std::vector(width, false));
    std::vector direction{0, 1, 2, 3};
    std::vector<QPoint> points;
    TIMER_START
    if (mazeMap.isHasBorder()) {
        points.emplace_back(1, 3);
        points.emplace_back(3, 1);
        vis[1][1] = true;
    } else {
        points.emplace_back(0, 2);
        points.emplace_back(2, 0);
        vis[0][0] = true;
    }
    while (!points.empty()) {
        distrib.param(stdParam{0, sc_int(points.size()) - 1});
        const int temp = distrib(gen);
        const QPoint tmp = points[temp];
        if (vis[tmp.x()][tmp.y()]) {
            points.erase(points.begin() + temp);
            continue;
        }
        vis[tmp.x()][tmp.y()] = true;
        std::ranges::shuffle(direction.begin(), direction.end(), gen);
        for (const auto dir: direction) {
            const auto t = QPoint(tmp.x() + fourDirs[dir][0], tmp.y() + fourDirs[dir][1]);
            if (mazeMap.judgePos(t.x(), t.y())) continue;
            if (vis[t.x()][t.y()]) {
                mazeMap.linkRoad(t.x(), t.y(), tmp.x(), tmp.y());
                break;
            }
        }
        for (const auto dir: fourDirs) {
            const auto t = QPoint(tmp.x() + dir[0], tmp.y() + dir[1]);
            if (mazeMap.judgePos(t.x(), t.y())) continue;
            if (vis[t.x()][t.y()]) continue;
            points.push_back(t);
        }
        points.erase(points.begin() + temp);
    }
    TIMER_STOP
    LOG(INFO) << "Prim generate finish";
    updateTime(time);
}
