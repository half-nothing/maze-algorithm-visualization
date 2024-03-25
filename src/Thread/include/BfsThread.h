/**********************************************
 * @file BfsThread.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef BFSTHREAD_H
#define BFSTHREAD_H

#include "Thread.h"

#include "Definition.h"

class BfsThread final : public Thread {
public:
    explicit BfsThread(const QPixmap &pixmap, const QPoint &start, const QPoint &end, QObject *parent = nullptr);

    [[nodiscard]] std::vector<Point> &getResult();

protected:
    void execute() override;

private:
    std::vector<Point> result{};
    const QPixmap &pixmap;
    QPoint start;
    QPoint end;
};

#endif //BFSTHREAD_H
