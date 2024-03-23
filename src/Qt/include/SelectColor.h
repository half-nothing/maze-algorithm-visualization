/**********************************************
 * @file SelectColor.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.24
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef SELECTCOLOR_H
#define SELECTCOLOR_H

#include <ConfigDefinition.h>
#include <QWidget>

namespace QT {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class SelectColor;
    }

    QT_END_NAMESPACE

    class SelectColor final : public QWidget {
        Q_OBJECT

    public:
        explicit SelectColor(QWidget *parent = nullptr);

        void setConfigField(ConfigField configField);

        ~SelectColor() override;

    protected:
        void mousePressEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;

    private:
        Ui::SelectColor *ui;
        QColor color;
        ConfigField configField = START_POINT_COLOR;
    };
}

#endif
