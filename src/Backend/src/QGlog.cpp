/**********************************************
 * @file QGlog.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.20
 * @license GNU General Public License (GPL)
 **********************************************/

#include "QGlog.h"

#include <qdir.h>

QGlog::QGlog(const char *name) {
    google::InitGoogleLogging(name);

    if (const QDir dir; !dir.exists("./logs")) dir.mkpath("./logs");

#ifndef NO_CONSOLE
    google::SetStderrLogging(google::INFO);
#endif

    google::SetLogDestination(google::INFO, "./logs/INFO_");
    google::SetLogDestination(google::WARNING, "./logs/WARNING_");
    google::SetLogDestination(google::ERROR, "./logs/ERROR_");
    google::SetLogFilenameExtension(".log");

    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 100;
    FLAGS_stop_logging_if_full_disk = true;
}

QGlog::~QGlog() {
    google::ShutdownGoogleLogging();
}
