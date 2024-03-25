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
#include "SearchThread.h"

#include "Definition.h"

class DfsThread final : public SearchThread {
public:
    DfsThread(const QPixmap &pixmap, const QPoint &start, const QPoint &end,
                       bool useStack = false, QObject *parent = nullptr);

    [[nodiscard]] std::vector<Point> &getResult() override;

    DfsThread(const DfsThread &other) = delete;

    DfsThread(DfsThread &&other) noexcept = delete;

    DfsThread & operator=(const DfsThread &other) = delete;

    DfsThread & operator=(DfsThread &&other) noexcept = delete;

protected:
    void execute() override;

private:
    bool useStack;
};

#endif
