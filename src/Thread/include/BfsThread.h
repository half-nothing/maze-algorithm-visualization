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

#include "SearchThread.h"

#include "Definition.h"

class BfsThread final : public SearchThread {
public:
    BfsThread(const QPixmap &pixmap, const QPoint &start, const QPoint &end, QObject * parent = nullptr);

    [[nodiscard]] std::vector<Point> &getResult() override;

    BfsThread(const BfsThread &other) = delete;

    BfsThread(BfsThread &&other) noexcept = delete;

    BfsThread & operator=(const BfsThread &other) = delete;

    BfsThread & operator=(BfsThread &&other) noexcept = delete;

protected:
    void execute() override;
};

#endif //BFSTHREAD_H
