#include "../include/BmpImage24Bit.h"

BmpImage24Bit::BmpImage24Bit(const std::string &filename) : BmpImage(filename) {
    readImage();
}

void BmpImage24Bit::saveImage(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::ate);
    file.write(reinterpret_cast<char *>(&bmpFile.fileHeader), sizeof(BmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpFile.infoHeader), sizeof(BmpInfoHeader));
    for (int i = bmpFile.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpFile.width; j++) {
            file.write(reinterpret_cast<char *>(bmpFile.pixels + bmpFile.width * i + j), sizeof(BmpPixel24Bit));
        }
        for (int j = 0; j < bmpFile.bytePerLine - bmpFile.width * 3; j++) {
            file.write("0", sizeof(uint8_t));
        }
    }
    file.close();
}

void BmpImage24Bit::readImage() {
    if (bmpFile.infoHeader.bitsPerPixel != 24) {
        std::cerr << "Not 24Bit Image." << std::endl;
        return;
    }

    bmpFile.bytePerLine = (bmpFile.width * 3 + 3) / 4 * 4;
    bmpFile.aligningOffset = bmpFile.bytePerLine - bmpFile.width * 3;

    inputFile.seekg(bmpFile.fileHeader.offsetSize, std::ios::beg);
    for (int i = bmpFile.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpFile.width; j++) {
            inputFile.read(reinterpret_cast<char *>(bmpFile.pixels + i * bmpFile.width + j), sizeof(BmpPixel24Bit));
        }
        inputFile.seekg(bmpFile.aligningOffset, std::ios::cur);
    }

    inputFile.close();
}

void BmpImage24Bit::toQPixMap(QPixmap &pixmap) {
    QImage qImage((uchar *) bmpFile.pixels, bmpFile.width, bmpFile.height, bmpFile.width * 3, QImage::Format_BGR888);
    pixmap = QPixmap::fromImage(qImage);
}

void BmpImage24Bit::bfs(QPainter &painter, QPoint start, QPoint end) {

}

void BmpImage24Bit::dfs(QPainter &painter, QPoint start, QPoint end) {

}

void BmpImage24Bit::aStar(QPainter &painter, QPoint start, QPoint end) {
}
