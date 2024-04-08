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
            if (step >= points.size()) {
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
        start = end = QPoint(0, 0);
        status = 0;
        points.clear();
        repaint();
        emit startPointUpdate("(0, 0)");
        emit endPointUpdate("(0, 0)");
    }

    void ImageDisplay::setSearchSequential(const bool searchSequential) {
        this->searchSequential = searchSequential;
        emit updateButtonStatus(searchSequential, searchSequential);
    }

    void ImageDisplay::setUseManhattan(const bool useManhattan) {
        this->useManhattan = useManhattan;
    }

    void ImageDisplay::updateShowImage(QPainter &painter) {
        if (imageWidth == 0 || imageHeight == 0) {
            return;
        }
        drawPixel(painter, start, Config::getInstance()->getConfigField(START_POINT_COLOR));
        drawPixel(painter, end, Config::getInstance()->getConfigField(END_POINT_COLOR));
        if (points.empty() || step < 0 || step >= points.size()) {
            return;
        }
        if (!this->searchSequential) {
            QPainter painter;
            painter.begin(&showImage);
            for (int i = step; i < points.size(); i++) {
                painter.setPen(QPen(Config::getInstance()->getConfigField(points[i].color)));
                painter.drawPoint(points[i].point);
            }
            painter.end();
            step = points.size();
            return;
        }
        if (points[step].point == start || points[step].point == end) {
            return;
        }
        QPainter imagePainter;
        imagePainter.begin(&showImage);
        if (backup) {
            imagePainter.setPen(QPen(Qt::transparent));
            imagePainter.drawPoint(points[step + 1].point);
            backup = false;
        }
        imagePainter.setPen(QPen(Config::getInstance()->getConfigField(points[step].color)));
        imagePainter.drawPoint(points[step].point);
        imagePainter.end();
    }

    void ImageDisplay::checkRangeLimit(QPoint &point) const {
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
                    start = getLocatePoint(event->position());
                    checkRangeLimit(start);
                    LOG(INFO) << "开始坐标: (" << sc_int(start.x()) << ", " << sc_int(start.y()) <<
                            ")"
                            << std::endl;
                    emit startPointUpdate(
                            QString::asprintf("(%d, %d)", sc_int(start.x()), sc_int(start.y())));
                    status++;
                    break;
                }
                case 1: {
                    end = getLocatePoint(event->position());
                    checkRangeLimit(end);
                    LOG(INFO) << "结束坐标: (" << sc_int(end.x()) << ", " << sc_int(end.y()) << ")"
                            << std::endl;
                    emit endPointUpdate(QString::asprintf("(%d, %d)", sc_int(end.x()), sc_int(end.y())));
                    status = 0;
                    break;
                }
                default: status = 0;
            }
        }
    }

    void ImageDisplay::dealDestroy() const {
        if (thread != nullptr) thread->stopThread();
    }

    void ImageDisplay::drawPathSlot() {
        timer->start();
    }

    void ImageDisplay::searchPath(const PathSearchMethod searchMethod) {
        thread = new SearchThread(searchMethod, backGroundImage, {sc_int(start.x()), sc_int(start.y())},
                                  {sc_int(end.x()), sc_int(end.y())}, useManhattan);
        connect(thread, &SearchThread::threadFinishSignal, [this] {
            points.clear();
            points = std::move(thread->getResult());
            showImage.fill(Qt::transparent);
            step = 0;
            emit updateButtonStatus(true, true);
            if (this->searchSequential) {
                emit drawPath();
                return;
            }
            repaint();
        });
        QThreadPool::globalInstance()->start(thread);
    }

    void ImageDisplay::stopPlay() {
        step = 0;
        if (timer->isActive()) {
            timer->stop();
        }
        showImage.fill(Qt::transparent);
        update();
    }

    void ImageDisplay::startPlay(const bool playing) {
        if (playing) {
            timer->stop();
            return;
        }
        timer->start();
    }
}
