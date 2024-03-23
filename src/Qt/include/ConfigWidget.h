/**********************************************
 * @file ConfigWidget.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.24
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>

namespace QT {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ConfigWidget;
    }

    QT_END_NAMESPACE

    class ConfigWidget final: public QWidget {
        Q_OBJECT

    public:
        explicit ConfigWidget(QWidget *parent = nullptr);

        ~ConfigWidget() override;

    private:
        Ui::ConfigWidget *ui;
    };
} // QT

#endif //CONFIGWIDGET_H
