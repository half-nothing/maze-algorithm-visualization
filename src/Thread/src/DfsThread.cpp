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

void DFSThread::execute() {
    GraphPath::getInstance()->dfs(pixmap, start, end);
    isRunning = false;
}
