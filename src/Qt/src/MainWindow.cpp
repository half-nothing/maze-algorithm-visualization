#include <BmpFactory.h>
#include <BmpImage.h>
#include <QFileDialog>
#include <QPalette>
#include <QStyleFactory>

#include "MainWindow.h"

#include <ConfigWidget.h>
#include <GraphPath.h>
#include <QMessageBox>
#include <QThreadPool>
#include <glog/logging.h>

#include "ui_MainWindow.h"

namespace QT {
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);
        configWidget = new ConfigWidget();
        setWindowTitle("Half_nothing");
        connect(this, SIGNAL(searchPathSignal(PathSearchMethod)), ui->image, SLOT(searchPath(PathSearchMethod)));
        connect(this, SIGNAL(destroyThread()), ui->image, SLOT(dealDestroy()));
        connect(ui->showSearchPathCheckBox, SIGNAL(clicked(bool)), ui->image, SLOT(setSearchSequential(bool)));
        connect(ui->useManhattanCheckBox, SIGNAL(clicked(bool)), ui->image, SLOT(setUseManhattan(bool)));
        connect(ui->searchDelaySlider, &QSlider::valueChanged, [this](const int value) {
            ui->searchDelaySpinBox->setValue(value);
            ui->image->setInterval(value);
        });
        connect(ui->searchDelaySpinBox, &QSpinBox::valueChanged, [this](const int value) {
            ui->searchDelaySlider->setValue(value);
            ui->image->setInterval(value);
        });
        connect(GraphPath::getInstance(), &GraphPath::updateCostTime, [this](const QString &value) {
            ui->searchCostValueLabel->setText(value);
        });
        connect(ui->aboutAction, &QAction::triggered, [this] {
            QMessageBox msgBox;
            msgBox.setWindowTitle("关于本软件");
            msgBox.setText("Half_nothing data structure course design homework.<hr/>"
                    "HomePage: <a href='https://half-nothing.cn/'>https://half-nothing.cn/</a><br/>"
                    "Github: <a href='https://github.com/half-nothing/DSCDesign'>https://github.com/half-nothing/DSCDesign</a><hr/>"
                    "Copyright © 2024 Half_nothing, All Rights Reserved.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        });
        connect(ui->optionWidgetAction, &QAction::triggered, [this] {
            if (!configWidget->isHidden()) {
                configWidget->hide();
            }
            configWidget->show();
        });
        connect(ui->exitAction, &QAction::triggered, this, &QWidget::close);
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

    void MainWindow::styleInit() {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/image/image/half128x128.png"), QSize(), QIcon::Normal, QIcon::Off);
        QApplication::setWindowIcon(icon);
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QPalette palette;
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::WindowText, Qt::white);
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Shadow, Qt::black);
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::ButtonText, Qt::white);
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Highlight, QColor(142, 45, 197));
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::HighlightedText, Qt::white);
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Text, Qt::white);
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Window, QColor(30, 30, 30));
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Button, QColor(30, 30, 30));
        palette.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Base, QColor(87, 87, 87));
        palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, QColor(54, 54, 54));
        palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, QColor(54, 54, 54));
        palette.setColor(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, QColor(54, 54, 54));
        QApplication::setPalette(palette);
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
                emit searchPathSignal(DFS_RECURSIVE);
                break;
            case 1:
                emit searchPathSignal(DFS_STACK);
                break;
            case 2:
                emit searchPathSignal(BFS);
                break;
            case 3:
                emit searchPathSignal(GBFS);
                break;
            case 4:
                emit searchPathSignal(DIJKSTRA);
                break;
            case 5:
                emit searchPathSignal(A_STAR);
                break;
            default: return;
        }
    }

    MainWindow::~MainWindow() {
        delete ui;
    }
}
