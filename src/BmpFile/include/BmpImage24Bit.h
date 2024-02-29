#ifndef DAC_BMPIMAGE24BIT_H
#define DAC_BMPIMAGE24BIT_H

#include <string>
#include "BmpImage.hpp"

class BmpImage24Bit : public BmpImage<BmpPixel24Bit> {
public:
    explicit BmpImage24Bit(const std::string &filename);

    void saveImage(const std::string &filename) override;

    void toQPixMap(QPixmap &pixmap) override;

    void bfs(QPainter &painter, QPoint start, QPoint end) override;

    void dfs(QPainter &painter, QPoint start, QPoint end) override;

    void aStar(QPainter &painter, QPoint start, QPoint end) override;

private:
    void readImage() override;
};

#endif
