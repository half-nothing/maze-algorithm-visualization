#ifndef DAC_IMAGEDISPLAY_H
#define DAC_IMAGEDISPLAY_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QWheelEvent>

namespace QT {
    QT_BEGIN_NAMESPACE
    namespace Ui { class ImageDisplay; }
    QT_END_NAMESPACE

    class ImageDisplay : public QWidget {
    Q_OBJECT

    public:
        explicit ImageDisplay(QWidget *parent = nullptr);

        ~ImageDisplay() override;

        void displayImage(QPixmap &Image);

    private:
        void paintEvent(QPaintEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void wheelEvent(QWheelEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        QPointF getLocate(QPointF pos);

        void drawPixel(int x, int y, QColor color);

        void paintGrid();

        size_t imageWidth = 188;
        size_t imageHeight = 120;
        float widthPerPix = 4;

        float zoom = 1;
        QPointF startPoint;

        bool leftMousePressed = false;
        QPointF preMousePos;
        QPointF nowMouseImagePos;
        QPixmap currentImage;
        Ui::ImageDisplay *ui;
    };
}

#endif
