/**********************************************
 * @file DfsThread.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.22
 * @license GNU General Public License (GPL)
 **********************************************/

#include "DfsThread.h"

#include <GraphPath.h>

DfsThread::DfsThread(const QPixmap &pixmap, const QPoint &start, const QPoint &end,
                     const bool useStack, QObject *const parent):
    Thread{parent}, pixmap{pixmap}, start{start}, end{end}, useStack{useStack} {}

std::vector<Point> &DfsThread::getResult() {
    return result;
}

void DfsThread::execute() {
    if (useStack) {
        GraphPath::getInstance()->dfsStackVersion(result, pixmap, start, end);
    } else {
        GraphPath::getInstance()->dfsRecursiveVersion(result, pixmap, start, end);
    }
    isRunning = false;
}
