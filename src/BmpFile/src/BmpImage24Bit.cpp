#include "../include/BmpImage24Bit.h"

BmpImage24Bit::BmpImage24Bit(const std::string &filename) : BmpImage(filename) {
    readImage();
}

void BmpImage24Bit::saveImage(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::ate);
    file.write(reinterpret_cast<char *>(&bmpFile.fileHeader), sizeof(BmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpFile.infoHeader), sizeof(BmpInfoHeader));
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            file.write(reinterpret_cast<char *>(bmpFile.pixels + width * i + j), sizeof(BmpPixel24Bit));
        }
        for (int j = 0; j < bytePerLine - width * 3; j++) {
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

    bytePerLine = (width * 3 + 3) / 4 * 4;

    inputFile.seekg(bmpFile.fileHeader.offsetSize, std::ios::beg);
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            inputFile.read(reinterpret_cast<char *>(bmpFile.pixels + i * width + j), sizeof(BmpPixel24Bit));
        }
        inputFile.seekg(bytePerLine - width * 3, std::ios::cur);
    }

    inputFile.close();
}

void BmpImage24Bit::toQPixMap(QPixmap &pixmap) {
    QImage qImage((uchar *) bmpFile.pixels, width, height, width * 3, QImage::Format_BGR888);
    pixmap = QPixmap::fromImage(qImage);
}
