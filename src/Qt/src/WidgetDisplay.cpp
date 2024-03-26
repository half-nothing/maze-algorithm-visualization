/**********************************************
 * @file WidgetDisplay.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#include "WidgetDisplay.h"
#include "Definition.h"

#include <QPainter>
#include <QWheelEvent>

WidgetDisplay::WidgetDisplay(QWidget *parent):
    QWidget(parent) {
    setMouseTracking(true);
}

WidgetDisplay::~WidgetDisplay() = default;

void WidgetDisplay::adjustZoom() {
    const double zoom_x = sc_double(this->width()) / imageWidth;
    const double zoom_y = sc_double(this->height()) / imageHeight;
    widthPerPix = zoom_x > zoom_y ? zoom_y : zoom_x;
    zoom = widthPerPix / 4.0;
    repaint();
}

void WidgetDisplay::resizeEvent(QResizeEvent *event) {
    update();
}

void WidgetDisplay::displayImage(const QPixmap &image) {
    if (image.width() != this->imageWidth) {
        this->imageWidth = image.width();
    }
    if (image.height() != this->imageHeight) {
        this->imageHeight = image.height();
    }
    currentImage = image;
    adjustZoom();
}

void WidgetDisplay::paintGrid() {
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

void WidgetDisplay::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(startPoint.x(), startPoint.y(),
                       imageWidth * widthPerPix, imageHeight * widthPerPix,
                       currentImage);
    drawPixel(nowMouseImagePos.x(), nowMouseImagePos.y(), QColor(255, 174, 201));
    paintGrid();
}

void WidgetDisplay::wheelEvent(QWheelEvent *event) {
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
    update();
}

void WidgetDisplay::mouseMoveEvent(QMouseEvent *event) {
    const auto nowpos = event->position();

    if (const auto tmp = this->getLocate(nowpos); tmp != nowMouseImagePos) {
        nowMouseImagePos = tmp;

        emit mousePointUpdate(QString::asprintf("(%d, %d)",
                                                static_cast<int>(nowMouseImagePos.x()),
                                                static_cast<int>(nowMouseImagePos.y())));
        update();
    }

    if (leftMousePressed) {
        startPoint += nowpos - preMousePos;
        update();
        preMousePos = nowpos;
    }
}

void WidgetDisplay::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }
    leftMousePressed = true;
    preMousePos = event->position();
}

void WidgetDisplay::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }
    leftMousePressed = false;
}

void WidgetDisplay::drawPixel(const int x, const int y, const QColor color) {
    if (x < 0 || y < 0 || x >= imageWidth || y >= imageHeight) return;
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(color);

    const QRectF PixelArea(startPoint.x() + x * widthPerPix, startPoint.y() + y * widthPerPix, widthPerPix,
                           widthPerPix);
    painter.drawRect(PixelArea);
}

QPointF WidgetDisplay::getLocate(const QPointF &pos) const {
    const QPointF tmp = pos - startPoint;
    return tmp / widthPerPix;
}

void WidgetDisplay::saveImage(const QString &filePath, const QImage::Format format) const {
    QImage image = currentImage.toImage();
    if (image.format() != format) {
        image = image.convertToFormat(format);
    }
    image.save(filePath, "BMP");
}
