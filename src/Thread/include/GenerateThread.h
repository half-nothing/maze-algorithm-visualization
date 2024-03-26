/**********************************************
 * @file GenerateThread.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef GENERATETHREAD_H
#define GENERATETHREAD_H
#include <MazeMap.h>
#include <Thread.h>

class GenerateThread final : public Thread {
public:
    GenerateThread(int row, int column, MazeGenerateMethod searchMethod, QObject *parent = nullptr);

    [[nodiscard]] MazeMap &getResult();

protected:
    void execute() override;

private:
    int row;
    int column;
    MazeGenerateMethod searchMethod;
    MazeMap result;
};

#endif
