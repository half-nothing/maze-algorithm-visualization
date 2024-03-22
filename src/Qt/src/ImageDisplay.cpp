#include "ImageDisplay.h"

#include <GraphPath.h>
#include <QThreadPool>

#include "ui_ImageDisplay.h"
#include "glog/logging.h"

namespace QT {
    ImageDisplay::ImageDisplay(QWidget *parent) :
        QWidget(parent), ui(new Ui::ImageDisplay) {
        ui->setupUi(this);
        timer = new QTimer(this);
        timer->setInterval(10);
        connect(timer, &QTimer::timeout, [this] {
            step++;
            if (step > points.size()) {
                timer->stop();
                return;
            }
            repaint();
        });
        connect(this, SIGNAL(drawPath()), this, SLOT(drawPathSlot()));
    }

    ImageDisplay::~ImageDisplay() {
        delete ui;
    }

    void ImageDisplay::resizeEvent(QResizeEvent *event) {
        this->repaint();
    }

    void ImageDisplay::displayImage(const QPixmap &image) {
        if (image.width() != this->imageWidth) {
            this->imageWidth = image.width();
        }
        if (image.height() != this->imageHeight) {
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

    void ImageDisplay::clearPoints() {
        start = end = QPointF(0, 0);
        set = 0;
        points.clear();
        repaint();
        emit startPointUpdate("(0, 0)");
        emit endPointUpdate("(0, 0)");
    }

    void ImageDisplay::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.drawPixmap(startPoint.x(), startPoint.y(),
                           imageWidth * widthPerPix, imageHeight * widthPerPix,
                           currentImage);
        drawPixel(nowMouseImagePos.x(), nowMouseImagePos.y(), QColor(255, 174, 201));
        drawMap();
        drawPixel(start.x(), start.y(), Qt::green);
        drawPixel(end.x(), end.y(), Qt::red);
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

    void ImageDisplay::checkRangeLimit(QPointF &point) const {
        if (point.x() < 0) {
            point.setX(0);
        }
        if (point.x() >= this->imageWidth) {
            point.setX(this->imageWidth - 1);
        }
        if (point.y() < 0) {
            point.setY(0);
        }
        if (point.y() >= this->imageHeight) {
            point.setY(this->imageHeight - 1);
        }
    }

    void ImageDisplay::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::RightButton) {
            switch (set) {
                case 0: {
                    start = getLocate(event->position());
                    checkRangeLimit(start);
                    LOG(INFO) << "开始坐标: (" << static_cast<int>(start.x()) << ", " << static_cast<int>(start.y()) <<
                            ")"
                            << std::endl;
                    emit startPointUpdate(
                            QString::asprintf("(%d, %d)", static_cast<int>(start.x()), static_cast<int>(start.y())));
                    set++;
                    break;
                }
                case 1: {
                    end = getLocate(event->position());
                    checkRangeLimit(end);
                    LOG(INFO) << "结束坐标: (" << static_cast<int>(end.x()) << ", " << static_cast<int>(end.y()) << ")"
                            << std::endl;
                    emit endPointUpdate(QString::asprintf("(%d, %d)", static_cast<int>(end.x()),
                                                          static_cast<int>(end.y())));
                    set = 0;
                    break;
                }
                default: set = 0;
            }
            return;
        }
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
        for (int i = 0; i < step && i < points.size(); i++) {
            drawPixel(points[i].point.x(), points[i].point.y(), points[i].color);
        }
    }

    QPointF ImageDisplay::getLocate(const QPointF &pos) const {
        const QPointF tmp = pos - startPoint;
        return tmp / widthPerPix;
    }

    void ImageDisplay::dealDestroy() const {
        if (dfsThread != nullptr) dfsThread->stopThread();
    }

    void ImageDisplay::drawPathSlot() {
        timer->start();
    }

    void ImageDisplay::dfsSearch() {
        dfsThread = new DFSThread(currentImage, {sc_int(start.x()), sc_int(start.y())},
                                  {sc_int(end.x()), sc_int(end.y())});
        connect(dfsThread, &DFSThread::threadFinishSignal, [this] {
            points.clear();
            step = 0;
            points = std::move(dfsThread->getResult());
            emit drawPath();
        });
        QThreadPool::globalInstance()->start(dfsThread);
    }
}
