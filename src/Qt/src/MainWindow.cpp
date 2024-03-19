#include "ui_MainWindow.h"
#include "MainWindow.h"
#include <QFileDialog>
#include <BmpImage8Bit.h>

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
        BmpImage8Bit image8Bit;
        const QString filePath = QFileDialog::getOpenFileName(this, tr("打开Bmp文件"), "./", tr("位图文件 (*.bmp)"));
        if (filePath.isEmpty()) {
            return;
        }
        image8Bit.readImage(filePath.toStdString());
        image8Bit.toQPixMap(pixmap);
        ui->image->displayImage(pixmap);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }
}
