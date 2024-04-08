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

        void startPlay(bool);

    public slots:
        void setSearchSequential(bool);

        void setUseManhattan(bool);

        void searchPath(PathSearchMethod);

        void dealDestroy() const;

        void drawPathSlot();

        void stopPlay();

    signals:
        void startPointUpdate(QString);

        void endPointUpdate(QString);

        void updateButtonStatus(bool, bool);

        void drawPath();

    protected:
        void updateShowImage(QPainter &painter) override;

    private:
        void checkRangeLimit(QPoint &point) const;

        void mousePressEvent(QMouseEvent *event) override;

        std::vector<Point> points;
        QPoint start;
        QPoint end;
        int status = 0;
        Ui::ImageDisplay *ui;
        SearchThread *thread = nullptr;
        QTimer *timer = nullptr;
        int step = 0;
        bool searchSequential = true;
        bool useManhattan = false;
        bool backup = false;
    };
}

#endif
