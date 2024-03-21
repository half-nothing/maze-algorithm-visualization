#ifndef DAC_MAINWINDOW_H
#define DAC_MAINWINDOW_H

#include <DfsThread.h>
#include <QMainWindow>
#include <Thread.h>

#include "ImageDisplay.h"

namespace QT {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class MainWindow;
    }

    QT_END_NAMESPACE

    class MainWindow final : public QMainWindow {
        Q_OBJECT

    public slots:
        void openImage();

        void findPath();

    signals:
        void dfsSignal();

        void destroyThread();

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        [[nodiscard]] ImageDisplay *getImageDisplay() const;

        ~MainWindow() override;

        void dealDestroy();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:
        Ui::MainWindow *ui;
        QPixmap pixmap;
    };
}

#endif
