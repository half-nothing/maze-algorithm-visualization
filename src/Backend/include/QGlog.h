/**********************************************
 * @file QGlog.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.20
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef QGLOG_H
#define QGLOG_H

#include <QObject>
#include "glog/logging.h"

class QGlog final : public QObject {
    Q_OBJECT

public:
    explicit QGlog(const char *name);

    ~QGlog() override;
};

#endif
