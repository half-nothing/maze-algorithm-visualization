#include "ui_MainWindow.h"
#include "MainWindow.h"


namespace QT {
    MainWindow::MainWindow(QWidget *parent) :
            QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }
}
