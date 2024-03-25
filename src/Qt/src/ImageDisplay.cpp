#include "ImageDisplay.h"

#include <BfsThread.h>
#include <Config.h>
#include <DfsThread.h>
#include <QThreadPool>

#include "ui_ImageDisplay.h"
#include "glog/logging.h"

namespace QT {
    ImageDisplay::ImageDisplay(QWidget *parent) :
        QWidget(parent), ui(new Ui::ImageDisplay) {
        ui->setupUi(this);
        timer = new QTimer(this);
        timer->setInterval(1);
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

    void ImageDisplay::setInterval(const int value) const {
        timer->setInterval(value);
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
        const double zoom_x = sc_double(this->width()) / imageWidth;
        const double zoom_y = sc_double(this->height()) / imageHeight;
        widthPerPix = zoom_x > zoom_y ? zoom_y : zoom_x;
        zoom = widthPerPix / 4.0;
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
        status = 0;
        points.clear();
        repaint();
        emit startPointUpdate("(0, 0)");
        emit endPointUpdate("(0, 0)");
    }

    void ImageDisplay::setSearchSequential(const bool searchSequential) {
        this->searchSequential = searchSequential;
    }

    void ImageDisplay::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.drawPixmap(startPoint.x(), startPoint.y(),
                           imageWidth * widthPerPix, imageHeight * widthPerPix,
                           currentImage);
        drawPixel(nowMouseImagePos.x(), nowMouseImagePos.y(), QColor(255, 174, 201));
        drawMap();
        drawPixel(start.x(), start.y(), Config::getInstance()->getConfigField(START_POINT_COLOR));
        drawPixel(end.x(), end.y(), Config::getInstance()->getConfigField(END_POINT_COLOR));
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

            emit mousePointUpdate(QString::asprintf("(%d, %d)",
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
            switch (status) {
                case 0: {
                    start = getLocate(event->position());
                    checkRangeLimit(start);
                    LOG(INFO) << "开始坐标: (" << static_cast<int>(start.x()) << ", " << static_cast<int>(start.y()) <<
                            ")"
                            << std::endl;
                    emit startPointUpdate(
                            QString::asprintf("(%d, %d)", static_cast<int>(start.x()), static_cast<int>(start.y())));
                    status++;
                    break;
                }
                case 1: {
                    end = getLocate(event->position());
                    checkRangeLimit(end);
                    LOG(INFO) << "结束坐标: (" << static_cast<int>(end.x()) << ", " << static_cast<int>(end.y()) << ")"
                            << std::endl;
                    emit endPointUpdate(QString::asprintf("(%d, %d)", static_cast<int>(end.x()),
                                                          static_cast<int>(end.y())));
                    status = 0;
                    break;
                }
                default: status = 0;
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
            drawPixel(points[i].point.x(), points[i].point.y(), Config::getInstance()->getConfigField(points[i].color));
        }
    }

    QPointF ImageDisplay::getLocate(const QPointF &pos) const {
        const QPointF tmp = pos - startPoint;
        return tmp / widthPerPix;
    }

    void ImageDisplay::dealDestroy() const {
        if (thread != nullptr) thread->stopThread();
    }

    void ImageDisplay::drawPathSlot() {
        timer->start();
    }

    void ImageDisplay::clearPath() {
        step = 0;
        repaint();
    }

    void ImageDisplay::repaintPath() {
        step = 0;
        timer->start();
    }

    void ImageDisplay::searchPath(const PathSearchMethod searchMethod) {
        switch (searchMethod) {
            case DFS_STACK: thread = new DfsThread(currentImage, {sc_int(start.x()), sc_int(start.y())},
                                                   {sc_int(end.x()), sc_int(end.y())}, true);
                break;
            case DFS_RECURSIVE: thread = new DfsThread(currentImage, {sc_int(start.x()), sc_int(start.y())},
                                                       {sc_int(end.x()), sc_int(end.y())}, false);
                break;
            case BFS: thread = new BfsThread(currentImage, {sc_int(start.x()), sc_int(start.y())},
                                             {sc_int(end.x()), sc_int(end.y())});
                break;
            case GBFS: return;
            case A_STAR: return;
            default: return;
        }
        connect(thread, &SearchThread::threadFinishSignal, [this] {
            points.clear();
            points = std::move(thread->getResult());
            if (this->searchSequential) {
                step = 0;
                emit drawPath();
                return;
            }
            step = points.size();
            repaint();
        });
        QThreadPool::globalInstance()->start(thread);
    }

}
