#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "BmpImage8Bit.h"
#include "BmpImage24Bit.h"
#include "BmpImage1Bit.h"


namespace QT {
    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
        QPixmap pixmap;
//        BmpImage8Bit image8Bit("map-8bit.bmp");
//        image8Bit.toQPixMap(pixmap);
//        BmpImage24Bit image24Bit("map-24bit.bmp");
//        image24Bit.toQPixMap(pixmap);
        BmpImage1Bit image1Bit;
        image1Bit.readImage("map-1bit.bmp");
        image1Bit.toQPixMap(pixmap);
        ui->image->displayImage(pixmap);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }
}
