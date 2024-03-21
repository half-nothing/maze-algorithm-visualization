/**********************************************
 * @file Thread.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.21
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef THREAD_H
#define THREAD_H
#include <QObject>
#include <QRunnable>
#include <Thread.h>

class Thread : public QObject, public QRunnable {
    Q_OBJECT

public slots:
    void run() final;

public:
    explicit Thread(QObject *parent = nullptr);

    void stopThread();

signals:
    void threadLoopSignal();
    void threadFinishSignal();

protected:
    virtual void beforeRunning();

    virtual void afterRunning();

    virtual void execute();

    bool isRunning;
};

#endif
