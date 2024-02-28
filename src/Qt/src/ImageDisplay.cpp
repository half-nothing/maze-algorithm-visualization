#include "ImageDisplay.h"
#include "ui_ImageDisplay.h"
#include "glog/logging.h"

namespace QT {
    ImageDisplay::ImageDisplay(QWidget *parent) :
            QWidget(parent), ui(new Ui::ImageDisplay) {
        ui->setupUi(this);
    }
    ImageDisplay::~ImageDisplay() {
        delete ui;
    }

    void ImageDisplay::resizeEvent(QResizeEvent *event) {
        this->repaint();
    }

    void ImageDisplay::displayImage(QPixmap &Image) {
        if (Image.width() != this->imageWidth || Image.height() != this->imageHeight) {
            return;
        }
        currentImage = Image;
        this->repaint();
    }

    void ImageDisplay::paintGrid() {
        QPainter painter(this);

        painter.setPen(QPen(Qt::black, 1));
        QList<QLineF> lines;

        for (double x = startPoint.x(); x <= startPoint.x() + widthPerPix * this->imageWidth; x += widthPerPix) {
            lines.append(QLineF(x, startPoint.y(), x, startPoint.y() + widthPerPix * this->imageHeight));
        }
        for (double y = startPoint.y(); y <= startPoint.y() + widthPerPix * this->imageHeight; y += widthPerPix) {
            lines.append(QLineF(startPoint.x(), y, startPoint.x() + widthPerPix * this->imageWidth, y));
        }
        painter.drawLines(lines);
    }

    void ImageDisplay::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.drawPixmap(startPoint.x(), startPoint.y(), imageWidth * widthPerPix, imageHeight * widthPerPix,
                           currentImage);

        drawPixel(nowMouseImagePos.x(), nowMouseImagePos.y(), QColor(255, 174, 201));
        this->paintGrid();
    }

    void ImageDisplay::wheelEvent(QWheelEvent *event) {
        event->angleDelta();
        if (event->angleDelta().y() > 0) {
            zoom += 0.1;
            widthPerPix = 4 * zoom;
        } else {
            zoom -= 0.1;

            if (zoom < 1.0) {
                zoom = 1.0;
            }
            widthPerPix = 4 * zoom;
        }
        this->repaint();
    }

    void ImageDisplay::mouseMoveEvent(QMouseEvent *event) {
        auto nowpos = event->position();

        auto tmp = this->getLocate(nowpos);

        if (tmp != nowMouseImagePos) {
            nowMouseImagePos = tmp;

            this->ui->cood->setText(QString().asprintf("图像坐标:(%d,%d)",
                                                       int(nowMouseImagePos.x()),
                                                       int(nowMouseImagePos.y())
            ));
            this->repaint();
        }

        if (leftMousePressed) {
            startPoint += nowpos - preMousePos;
            this->repaint();
            preMousePos = nowpos;
        }
    }

    void ImageDisplay::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            leftMousePressed = true;
            preMousePos = event->position();
        }
    }

    void ImageDisplay::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            leftMousePressed = false;
        }
    }

    void ImageDisplay::drawPixel(int x, int y, QColor color) {
        if (x < 0 || y < 0 || x >= imageWidth || y >= imageHeight)return;
        QPainter painter(this);
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(color);

        QRectF PixelArea(startPoint.x() + x * widthPerPix, startPoint.y() + y * widthPerPix, widthPerPix, widthPerPix);
        painter.drawRect(PixelArea);
    }

    QPointF ImageDisplay::getLocate(QPointF pos) {
        QPointF tmp = pos - startPoint;
        return tmp / widthPerPix;
    }

}
