#include <BmpFactory.h>
#include <BmpImage.h>
#include <QFileDialog>

#include "MainWindow.h"

#include <GraphPath.h>
#include <QMessageBox>
#include <QThreadPool>
#include <glog/logging.h>

#include "ui_MainWindow.h"

namespace QT {
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
        setWindowTitle("Half_nothing");
        connect(this, SIGNAL(dfsSignal()), ui->image, SLOT(dfsSearch()));
        connect(this, SIGNAL(destroyThread()), ui->image, SLOT(dealDestroy()));
        connect(ui->showSearchPathCheckBox, SIGNAL(clicked(bool)), ui->image, SLOT(setSearchSequential(bool)));
        connect(ui->searchDelaySlider, &QSlider::valueChanged, [this](const int value) {
            ui->searchDelaySpinBox->setValue(value);
            ui->image->setInterval(value);
        });
        connect(ui->searchDelaySpinBox, &QSpinBox::valueChanged, [this](const int value) {
            ui->searchDelaySlider->setValue(value);
            ui->image->setInterval(value);
        });
        connect(GraphPath::getInstance(), &GraphPath::updateCostTime, [this](const QString& value) {
            ui->searchCostValueLabel->setText(value);
        });
        LOG(INFO) << "Main thread: " << QThread::currentThread();
    }

    void MainWindow::closeEvent(QCloseEvent *event) {
        LOG(INFO) << "Request to quit";
        QMessageBox msgBox;
        msgBox.setWindowTitle("提示");
        msgBox.setText("确认退出?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if (const int ret = msgBox.exec(); ret == QMessageBox::Ok) {
            QMainWindow::closeEvent(event);
            dealDestroy();
            event->accept();
            return;
        }
        event->ignore();
    }

    void MainWindow::dealDestroy() {
        emit destroyThread();
    }

    ImageDisplay *MainWindow::getImageDisplay() const {
        return ui->image;
    }

    void MainWindow::openImage() {
        ui->image->clearPoints();
        const QString filePath = QFileDialog::getOpenFileName(this, tr("打开Bmp文件"), "./", tr("位图文件 (*.bmp)"));
        if (filePath.isEmpty()) {
            return;
        }
        BmpImage *image = BmpFactory::createBmpImage(filePath.toStdString());
        image->readImage(filePath.toStdString());
        image->toQPixMap(pixmap);
        ui->image->displayImage(pixmap);
    }

    void MainWindow::findPath() {
        switch (ui->pathFindingComboBox->currentIndex()) {
            case 0:
                LOG(INFO) << "BFS";
                break;
            case 1:
                LOG(INFO) << "DFS";
                emit dfsSignal();
                break;
            case 2:
                LOG(INFO) << "A*";
                break;
            default: return;
        }
    }

    MainWindow::~MainWindow() {
        delete ui;
    }
}
