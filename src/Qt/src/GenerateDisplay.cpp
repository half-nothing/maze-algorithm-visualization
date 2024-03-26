/**********************************************
 * @file GenerateDisplay.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "GenerateDisplay.h"
#include "ui_GenerateDisplay.h"

namespace QT {
    GenerateDisplay::GenerateDisplay(QWidget *parent) :
        WidgetDisplay(parent), ui(new Ui::GenerateDisplay) {
        ui->setupUi(this);
        imageHeight = 50;
        imageWidth = 50;
        adjustZoom();
    }

    GenerateDisplay::~GenerateDisplay() {
        delete ui;
    }

    void GenerateDisplay::setHeight(const int height) {
        imageHeight = height;
        adjustZoom();
    }
    void GenerateDisplay::setWidth(const int width) {
        imageWidth = width;
        adjustZoom();
    }
}
