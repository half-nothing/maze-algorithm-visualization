#include "ImageDisplay.h"

#include <Config.h>
#include <QThreadPool>

#include "ui_ImageDisplay.h"
#include "glog/logging.h"

namespace QT {
    ImageDisplay::ImageDisplay(QWidget *parent) :
        WidgetDisplay(parent), ui(new Ui::ImageDisplay) {
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

    void ImageDisplay::setUseManhattan(const bool useManhattan) {
        this->useManhattan = useManhattan;
    }

    void ImageDisplay::paintEvent(QPaintEvent *event) {
        WidgetDisplay::paintEvent(event);
        QPainter painter(this);
        painter.setPen(QPen(Qt::black, 1));
        drawMap(painter);
        drawPixel(painter, start, QColor(Config::getInstance()->getConfigField(START_POINT_COLOR)));
        drawPixel(painter, end, QColor(Config::getInstance()->getConfigField(END_POINT_COLOR)));
        painter.end();
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
        WidgetDisplay::mousePressEvent(event);
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
        }
    }

    void ImageDisplay::drawMap(QPainter &painter) {
        for (int i = 0; i < step && i < points.size(); i++) {
            drawPixel(painter, points[i].point, Config::getInstance()->getConfigField(points[i].color));
        }
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
        thread = new SearchThread(searchMethod, currentImage, {sc_int(start.x()), sc_int(start.y())},
                                  {sc_int(end.x()), sc_int(end.y())}, useManhattan);
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
