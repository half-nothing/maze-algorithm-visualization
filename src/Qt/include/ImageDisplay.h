#ifndef DAC_IMAGEDISPLAY_H
#define DAC_IMAGEDISPLAY_H

#include <Definition.h>
#include <QTimer>
#include <QPainter>
#include <QWheelEvent>
#include <SearchThread.h>
#include <WidgetDisplay.h>

namespace QT {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class ImageDisplay;
    }

    QT_END_NAMESPACE

    class ImageDisplay final : public WidgetDisplay {
        Q_OBJECT

    public:
        explicit ImageDisplay(QWidget *parent = nullptr);

        ~ImageDisplay() override;

        void clearPoints();

        void setInterval(int value) const;

    public slots:
        void setSearchSequential(bool);

        void setUseManhattan(bool);

        void searchPath(PathSearchMethod);

        void dealDestroy() const;

        void drawPathSlot();

        void clearPath();

        void repaintPath();

    signals:
        void startPointUpdate(QString);

        void endPointUpdate(QString);

        void drawPath();

    private:
        void paintEvent(QPaintEvent *event) override;

        void checkRangeLimit(QPointF &point) const;

        void mousePressEvent(QMouseEvent *event) override;

        void drawMap(QPainter &painter);

        std::vector<Point> points;
        QPointF start;
        QPointF end;
        int status = 0;
        Ui::ImageDisplay *ui;
        SearchThread *thread = nullptr;
        QTimer *timer = nullptr;
        int step = 0;
        bool searchSequential = true;
        bool useManhattan = false;
    };
}

#endif
