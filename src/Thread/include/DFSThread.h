/**********************************************
 * @file DfsThread.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.22
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef DFSTHREAD_H
#define DFSTHREAD_H
#include "Thread.h"

#include "Definition.h"

class DFSThread final : public Thread {
public:
    explicit DFSThread(const QPixmap &pixmap, const QPoint &start, const QPoint &end,
                       bool useStack = false, QObject *parent = nullptr);

    [[nodiscard]] std::vector<Point> &getResult();

protected:
    void execute() override;

private:
    std::vector<Point> result{};
    const QPixmap &pixmap;
    QPoint start;
    QPoint end;
    bool useStack;
};

#endif
