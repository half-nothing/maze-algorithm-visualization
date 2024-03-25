/**********************************************
 * @file BfsThread.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "BfsThread.h"

#include <GraphPath.h>

BfsThread::BfsThread(const QPixmap &pixmap, const QPoint &start, const QPoint &end, QObject *const parent):
    Thread{parent}, pixmap{pixmap}, start{start}, end{end} {}

std::vector<Point> &BfsThread::getResult() {
    return result;
}

void BfsThread::execute() {
    GraphPath::getInstance()->bfs(result, pixmap, start, end);
    isRunning = false;
}
