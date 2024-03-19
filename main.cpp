#include <BmpImage8Bit.h>
#include <GraphPath.h>
#include <QApplication>
#include "MainWindow.h"
#include "glog/logging.h"

int main(int argc, char *argv[]) {
#if WIN64 || WIN32
    system("chcp 65001");
#endif
    QApplication a(argc, argv);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO);
    LOG(INFO) << "Application start" << std::endl;
    QT::MainWindow w;
    w.show();
    // GraphPath::dfs(w.getImageDisplay(), pixmap, {2, 2}, {8, 2});
    const int code = QApplication::exec();
    LOG(INFO) << "Application stop" << std::endl;
    return code;
}
