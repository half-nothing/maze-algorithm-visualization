#include <QApplication>
#include "MainWindow.h"
#include "glog/logging.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    LOG(INFO) << "Application start" << std::endl;
    QT::MainWindow w;
    w.show();
    int code = QApplication::exec();
    LOG(INFO) << "Application stop" << std::endl;
    return code;
}
