/**********************************************
 * @file SelectColor.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.24
 * @license GNU General Public License (GPL)
 **********************************************/

#include "SelectColor.h"

#include <Config.h>
#include <QColorDialog>
#include <QPainter>
#include <qevent.h>
#include <qstyleoption.h>

#include "ui_SelectColor.h"

namespace QT {
    SelectColor::SelectColor(QWidget *parent) :
        QWidget(parent), ui(new Ui::SelectColor) {
        ui->setupUi(this);
    }

    void SelectColor::setConfigField(const ConfigField configField) {
        this->configField = configField;
        color = Config::getInstance()->getConfigField(configField);
        setStyleSheet("background-color: " + color.name() + ";");
        update();
    }

    SelectColor::~SelectColor() {
        delete ui;
    }

    void SelectColor::paintEvent(QPaintEvent *event) {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

    void SelectColor::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            const QColor color = QColorDialog::getColor(Qt::white);
            setColor(color);
        }
    }

    void SelectColor::setColor(const QColor &color) {
        if (!color.isValid()) {
            return;
        }
        if (color == this->color) {
            return;
        }
        setStyleSheet("background-color: " + color.name() + ";");
        this->color = color;
        Config::getInstance()->setConfigField(configField, color);
        update();
    }
}
