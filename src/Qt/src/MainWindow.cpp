#include "ui_MainWindow.h"
#include "MainWindow.h"

#include <BmpFactory.h>
#include <BmpImage.h>
#include <QFileDialog>

namespace QT {
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
    }

    ImageDisplay *MainWindow::getImageDisplay() const {
        return ui->image;
    }

    void MainWindow::openImage() {
        QPixmap pixmap;
        const QString filePath = QFileDialog::getOpenFileName(this, tr("打开Bmp文件"), "./", tr("位图文件 (*.bmp)"));
        if (filePath.isEmpty()) {
            return;
        }
        BmpImage *image = BmpFactory::createBmpImage(filePath.toStdString());
        image->readImage(filePath.toStdString());
        image->toQPixMap(pixmap);
        ui->image->displayImage(pixmap);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }
}
