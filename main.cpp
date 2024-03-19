#include <BmpImage8Bit.h>
#include <GraphPath.h>
#include <QApplication>
#include "MainWindow.h"
#include "glog/logging.h"
#include "CircularlyLinkList.h"

int main(int argc, char *argv[]) {
#if WIN64 || WIN32
    system("chcp 65001");
#endif
    QApplication a(argc, argv);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    LOG(INFO) << "Application start" << std::endl;
    QT::MainWindow w;
    QPixmap pixmap;
    BmpImage8Bit image8Bit;
    image8Bit.readImage("map-8bit.bmp");
    image8Bit.toQPixMap(pixmap);
    w.getImageDisplay()->displayImage(pixmap);
    w.show();
    // GraphPath::dfs(w.getImageDisplay(), pixmap, {2, 2}, {8, 2});
    int code = QApplication::exec();
    LOG(INFO) << "Application stop" << std::endl;
    return code;
}
