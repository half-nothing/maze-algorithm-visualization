#ifndef DAC_MAINWINDOW_H
#define DAC_MAINWINDOW_H

#include <QMainWindow>
#include "ImageDisplay.h"

namespace QT {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private:
        Ui::MainWindow *ui;
    };
}

#endif
