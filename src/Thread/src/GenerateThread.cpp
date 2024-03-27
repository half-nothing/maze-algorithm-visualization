/**********************************************
 * @file GenerateThread.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "GenerateThread.h"

#include <MazeGenerator.h>

GenerateThread::GenerateThread(const int row, const int column, const MazeGenerateMethod searchMethod,
                               QObject *const parent):
    Thread{parent}, row{row}, column{column}, searchMethod{searchMethod}, result{row, column} {
    result.mapInit();
}

MazeMap &GenerateThread::getResult() {
    return result;
}

void GenerateThread::execute() {
    switch (searchMethod) {
        case DFS: MazeGenerator::getInstance()->DFS(result);
            break;
        case PRIM: MazeGenerator::getInstance()->Prim(result);
            break;
        default: break;
    }
    isRunning = false;
}
