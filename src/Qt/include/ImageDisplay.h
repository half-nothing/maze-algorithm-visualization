#ifndef DAC_IMAGEDISPLAY_H
#define DAC_IMAGEDISPLAY_H

#include <Definition.h>
#include <DfsThread.h>
#include <QWidget>
#include <QImage>
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

        void delPoint(const Point &point);

        void clearPoints();

    public slots:
        void dfsSearch();

        void addPoint(Point);

        void dealDestroy() const;

    signals:
        void startPointUpdate(QString);

        void endPointUpdate(QString);

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

        size_t imageWidth = 188;
        size_t imageHeight = 120;
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
        int set = 0;
        Ui::ImageDisplay *ui;
        DFSThread *dfsThread = nullptr;
    };
}

#endif
