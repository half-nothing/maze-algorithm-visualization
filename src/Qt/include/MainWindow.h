#ifndef DAC_MAINWINDOW_H
#define DAC_MAINWINDOW_H

#include <ConfigWidget.h>
#include <GenerateMaze.h>
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

        void saveImage();

        void updateButton(bool, bool);

    signals:
        void searchPathSignal(PathSearchMethod);

        void destroyThread();

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        [[nodiscard]] ImageDisplay *getImageDisplay() const;

        ~MainWindow() override;

        void dealDestroy();

        static void styleInit();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:
        Ui::MainWindow *ui;
        ConfigWidget *configWidget;
        GenerateMaze *generateMaze;
        QPixmap pixmap;
        bool playing = false;
    };
}

#endif
