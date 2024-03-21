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
    QApplication a(argc, argv);
    LOG(INFO) << "Application start" << std::endl;
    QT::MainWindow w;
    w.show();
    const int code = QApplication::exec();
    LOG(INFO) << "Application stop" << std::endl;
    return code;
}
