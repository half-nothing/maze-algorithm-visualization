/**********************************************
 * @file SearchThread.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "SearchThread.h"

#include <GraphPath.h>

SearchThread::SearchThread(const PathSearchMethod &method, const QPixmap &pixmap, const QPoint &start,
                           const QPoint &end, QObject *const parent):
    Thread{parent}, searchMethod{method}, pixmap{pixmap}, start{start}, end{end} {}

std::vector<Point> &SearchThread::getResult() {
    return result;
}

void SearchThread::execute() {
    switch (searchMethod) {
        case DFS_STACK: GraphPath::getInstance()->DFSStackVersion(result, pixmap, start, end);
            break;
        case DFS_RECURSIVE: GraphPath::getInstance()->DFSRecursiveVersion(result, pixmap, start, end);
            break;
        case BFS: GraphPath::getInstance()->BFS(result, pixmap, start, end);
            break;
        case GBFS: GraphPath::getInstance()->GBFS(result, pixmap, start, end);
            break;
        case DIJKSTRA: GraphPath::getInstance()->Dijkstra(result, pixmap, start, end);
            break;
        case A_STAR: break;
        default: break;
    }
    isRunning = false;
}
