/**********************************************
 * @file Thread.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.21
 * @license GNU General Public License (GPL)
 **********************************************/

#include "Thread.h"

#include <MainWindow.h>
#include <QThread>

#include <glog/logging.h>

Thread::Thread(QObject *parent):
    QObject{parent} {
    isRunning = true;
}

void Thread::stopThread() {
    isRunning = false;
}

void Thread::run() {
    LOG(INFO) << "Sub thread id: " << QThread::currentThreadId();
    beforeRunning();
    while (isRunning) {
        execute();
        emit threadLoopSignal();
        if (isRunning == false) {
            break;
        }
    }
    afterRunning();
    emit threadFinishSignal();
    LOG(INFO) << "Sub thread exit";
}

void Thread::beforeRunning() {}
void Thread::afterRunning() {}

void Thread::execute() {
    QThread::sleep(1);
}
