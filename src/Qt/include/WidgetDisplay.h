/**********************************************
 * @file WidgetDisplay.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef WIDGETDISPLAY_H
#define WIDGETDISPLAY_H
#include <QWidget>

class WidgetDisplay : public QWidget {
    Q_OBJECT

public:
    explicit WidgetDisplay(QWidget *parent = nullptr);

    ~WidgetDisplay() override;

    void displayImage(const QPixmap &image);

    void adjustZoom();

    void saveImage(const QString &filePath, QImage::Format format) const;

    [[nodiscard]] const QPixmap &getCurrentImage() const;

signals:
    void mousePointUpdate(QString);

protected:
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    [[nodiscard]] QPointF getLocate(const QPointF &pos) const;

    [[nodiscard]] QPoint getLocatePoint(const QPointF &pos) const;

    void paintGrid(QPainter &painter) const;

    void drawPixel(QPainter &painter, const QPointF &point, QColor color) const;

    virtual void updateShowImage(QPainter &painter);

    size_t imageWidth = 0;
    size_t imageHeight = 0;
    float widthPerPix = 4;
    float zoom = 1;
    QPointF startPoint;
    bool leftMousePressed = false;
    QPointF preMousePos;
    QPointF nowMouseImagePos;
    QPixmap backGroundImage;
    QPixmap showImage;
    bool keepStatic = false;
};

#endif //WIDGETDISPLAY_H
