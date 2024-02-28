//#include "MainWindow.h"
//#include "SequentialStack.hpp"
//#include <QApplication>
//#include <QDebug>
#include "glog/logging.h"
#include "BmpImage1Bit.h"

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    LOG(INFO) << "TEST";
    BmpImage1Bit bmpImage1Bit("test2.bmp");
//    bmpImage1Bit.printImage();
//    Utils::createDirectory("log/log/log/log");
//    QApplication a(argc, argv);
//    qDebug() << "Hello World";
//    QT::MainWindow w;
//    SequentialStack<int> sequentialStack({1, 2, 3, 4, 5});
//    logger << sequentialStack;
//    w.show();
//    return QApplication::exec();
}
