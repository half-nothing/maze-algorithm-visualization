#include <Config.h>
#include <QApplication>
#include <QGlog.h>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
#if WIN64 || WIN32
#ifndef NO_CONSOLE
    system("chcp 65001");
#endif
#endif
    QGlog glog("logs");
    Config::initConfig();
    LOG(INFO) << "Application start" << std::endl;
    QApplication app(argc, argv);
    QT::MainWindow::styleInit();
    QT::MainWindow w;
    w.show();
    const int code = QApplication::exec();
    LOG(INFO) << "Application stop" << std::endl;
    return code;
}
