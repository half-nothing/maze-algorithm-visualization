/**********************************************
 * @file GenerateMaze.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "GenerateMaze.h"

#include <MazeGenerator.h>
#include <QFileDialog>

#include "ui_GenerateMaze.h"

namespace QT {
    GenerateMaze::GenerateMaze(QWidget *parent) :
        QWidget(parent), ui(new Ui::GenerateMaze) {
        ui->setupUi(this);
        connect(this, SIGNAL(generateMazeSignal(MazeGenerateMethod)), ui->image,
                SLOT(generateMaze(MazeGenerateMethod)));
        connect(ui->sendToMainPageButton, &QPushButton::clicked, [this] {
            emit sendToMainPage(ui->image->getCurrentImage());
        });
        connect(MazeGenerator::getInstance(), &MazeGenerator::updateCostTime, [this](const QString &value) {
            ui->generateTimeCostValueLabel->setText(value);
        });
    }

    GenerateMaze::~GenerateMaze() {
        delete ui;
    }

    void GenerateMaze::saveMaze() {
        const QString filePath = QFileDialog::getSaveFileName(this, tr("要保存文件的位置"), "./", tr("位图文件 (*.bmp)"));
        if (filePath.isEmpty()) {
            return;
        }
        switch (ui->saveTypeComboBox->currentIndex()) {
            case 0: ui->image->saveImage(filePath, QImage::Format_Mono);
                break;
            case 1: ui->image->saveImage(filePath, QImage::Format_Indexed8);
                break;
            case 2: ui->image->saveImage(filePath, QImage::Format_RGB888);
                break;
            default: return;
        }
    }

    void GenerateMaze::generateMaze() {
        switch (ui->generateMethodComboBox->currentIndex()) {
            case 0:
                emit generateMazeSignal(DFS);
                break;
            case 1:
                emit generateMazeSignal(PRIM);
                break;
            default: return;
        }
    }
}
