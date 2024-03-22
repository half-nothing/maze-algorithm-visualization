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
#include "Definition.h"
#include <Thread.h>

class DFSThread final : public Thread {
public:
    explicit DFSThread(const QPixmap &pixmap,
                       const QPoint &start,
                       const QPoint &end,
                       QObject *const parent = nullptr) :
        Thread{parent},
        pixmap{pixmap},
        start{start},
        end{end} {}

    [[nodiscard]] std::vector<Point> &getResult();

protected:
    void execute() override;

private:
    std::vector<Point> result{};
    const QPixmap &pixmap;
    QPoint start;
    QPoint end;
};

#endif
