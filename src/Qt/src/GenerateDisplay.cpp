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

#include <Config.h>
#include <QPainter>
#include <QThreadPool>

#include "ui_GenerateDisplay.h"

namespace QT {
    GenerateDisplay::GenerateDisplay(QWidget *parent) :
        WidgetDisplay(parent), ui(new Ui::GenerateDisplay) {
        ui->setupUi(this);
        imageHeight = 50;
        imageWidth = 50;
        backGroundImage = QPixmap(imageWidth, imageHeight);
        backGroundImage.fill(Config::getInstance()->getConfigField(WALL_COLOR));
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

    void GenerateDisplay::generateMaze(const MazeGenerateMethod method) {
        thread = new GenerateThread(imageHeight, imageWidth, method);
        connect(thread, &GenerateThread::threadFinishSignal, [this] {
            result = std::move(thread->getResult());
            backGroundImage = result.toQPixmap();
            update();
        });
        QThreadPool::globalInstance()->start(thread);
    }
}
