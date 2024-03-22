#ifndef DAC_IMAGEDISPLAY_H
#define DAC_IMAGEDISPLAY_H

#include <Definition.h>
#include <DfsThread.h>
#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QWheelEvent>

#include "DfsThread.h"

namespace QT {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ImageDisplay;
    }

    QT_END_NAMESPACE

    class ImageDisplay final : public QWidget {
        Q_OBJECT

    public:
        explicit ImageDisplay(QWidget *parent = nullptr);

        ~ImageDisplay() override;

        void displayImage(const QPixmap &image);

        void clearPoints();

        void setInterval(int value) const;

    public slots:
        void setSearchSequential(bool);

        void dfsSearch();

        void dealDestroy() const;

        void drawPathSlot();

        void clearPath();

        void repaintPath();

    signals:
        void startPointUpdate(QString);

        void endPointUpdate(QString);

        void mousePointUpdate(QString);

        void drawPath();

    private:
        void paintEvent(QPaintEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void wheelEvent(QWheelEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void checkRangeLimit(QPointF &point) const;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        QPointF getLocate(const QPointF &pos) const;

        void drawPixel(int x, int y, QColor color);

        void drawMap();

        void paintGrid();

        size_t imageWidth = 0;
        size_t imageHeight = 0;
        float widthPerPix = 4;

        float zoom = 1;
        QPointF startPoint;

        bool leftMousePressed = false;

        std::vector<Point> points;
        QPointF preMousePos;
        QPointF nowMouseImagePos;
        QPixmap currentImage;
        QPointF start;
        QPointF end;
        int status = 0;
        Ui::ImageDisplay *ui;
        DFSThread *dfsThread = nullptr;
        QTimer *timer = nullptr;
        int step = 0;
        bool searchSequential = true;
    };
}

#endif
