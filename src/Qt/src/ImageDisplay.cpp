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

    void ImageDisplay::displayImage(const QPixmap &image) {
        if (image.width() != this->imageWidth || image.height() != this->imageHeight) {
            this->imageWidth = image.width();
            this->imageHeight = image.height();
        }
        currentImage = image;
        this->repaint();
    }

    void ImageDisplay::paintGrid() {
        if (zoom < 1) {
            return;
        }
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

    const std::vector<Point> &ImageDisplay::getPoints() const {
        return points;
    }

    void ImageDisplay::addPoint(const Point &point) {
        this->points.push_back(point);
        this->repaint();
    }

    void ImageDisplay::delPoint(const Point &point) {
        if (const auto it =
                    std::find(this->points.begin(),
                              this->points.end(),
                              point);
            it != this->points.end()) {
            this->points.erase(it);
            this->repaint();
        }
    }

    void ImageDisplay::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.drawPixmap(startPoint.x(), startPoint.y(),
                           imageWidth * widthPerPix, imageHeight * widthPerPix,
                           currentImage);
        drawPixel(nowMouseImagePos.x(), nowMouseImagePos.y(), QColor(255, 174, 201));
        drawMap();
        this->paintGrid();
    }

    void ImageDisplay::wheelEvent(QWheelEvent *event) {
        event->angleDelta();
        if (event->angleDelta().y() > 0) {
            zoom += 0.1;
            widthPerPix = 4 * zoom;
            this->repaint();
            return;
        }
        zoom -= 0.1;
        if (zoom < 0.2) {
            zoom = 0.2;
        }
        widthPerPix = 4 * zoom;
        this->repaint();
    }

    void ImageDisplay::mouseMoveEvent(QMouseEvent *event) {
        const auto nowpos = event->position();

        if (const auto tmp = this->getLocate(nowpos); tmp != nowMouseImagePos) {
            nowMouseImagePos = tmp;

            this->ui->cood->setText(QString::asprintf("图像坐标:(%d,%d)",
                                                      static_cast<int>(nowMouseImagePos.x()),
                                                      static_cast<int>(nowMouseImagePos.y())));
            this->repaint();
        }

        if (leftMousePressed) {
            startPoint += nowpos - preMousePos;
            this->repaint();
            preMousePos = nowpos;
        }
    }

    void ImageDisplay::mousePressEvent(QMouseEvent *event) {
        if (event->button() != Qt::LeftButton) {
            return;
        }
        leftMousePressed = true;
        preMousePos = event->position();
    }

    void ImageDisplay::mouseReleaseEvent(QMouseEvent *event) {
        if (event->button() != Qt::LeftButton) {
            return;
        }
        leftMousePressed = false;
    }

    void ImageDisplay::drawPixel(const int x, const int y, const QColor color) {
        if (x < 0 || y < 0 || x >= imageWidth || y >= imageHeight) return;
        QPainter painter(this);
        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(color);

        const QRectF PixelArea(startPoint.x() + x * widthPerPix, startPoint.y() + y * widthPerPix, widthPerPix,
                               widthPerPix);
        painter.drawRect(PixelArea);
    }

    void ImageDisplay::drawMap() {
        for (auto &[point, color]: this->points) {
            drawPixel(point.x(), point.y(), color);
        }
    }

    QPointF ImageDisplay::getLocate(const QPointF &pos) const {
        const QPointF tmp = pos - startPoint;
        return tmp / widthPerPix;
    }

}
