/**********************************************
 * @file SearchThread.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef SEARCHPATHTHREAD_H
#define SEARCHPATHTHREAD_H
#include <Thread.h>

class SearchThread : public Thread {
public:
    SearchThread(const QPixmap &pixmap, const QPoint &start, const QPoint &end, QObject *parent = nullptr) ;

    [[nodiscard]] virtual std::vector<Point> &getResult() = 0;

    SearchThread(const SearchThread &other) = delete;

    SearchThread(SearchThread &&other) noexcept = delete;

    SearchThread & operator=(const SearchThread &other) = delete;

    SearchThread & operator=(SearchThread &&other) noexcept = delete;

protected:
    std::vector<Point> result{};
    const QPixmap &pixmap;
    QPoint start;
    QPoint end;
};

#endif
