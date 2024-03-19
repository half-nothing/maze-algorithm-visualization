#include "BmpImage24Bit.h"

#include <glog/logging.h>

BmpImage24Bit::BmpImage24Bit() :
    BmpImage() {}

BmpImage24Bit::~BmpImage24Bit() {
    delete [] bmpPixelInfo.pixels;
    delete [] bmpPixelInfo.palettes;
}

void BmpImage24Bit::saveImage(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::ate);
    file.write(reinterpret_cast<char *>(&bmpFileHeader), sizeof(BmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(BmpInfoHeader));
    for (int i = bmpPixelInfo.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpPixelInfo.width; j++) {
            file.write(reinterpret_cast<char *>(bmpPixelInfo.pixels + bmpPixelInfo.width * i + j), sizeof(BmpPixel24Bit));
        }
        for (int j = 0; j < bmpPixelInfo.bytePerLine - bmpPixelInfo.width * 3; j++) {
            file.write("0", sizeof(uint8_t));
        }
    }
    file.close();
}

void BmpImage24Bit::readImage(const std::string &filename) {
    BmpImage::readImage(filename);

    bmpPixelInfo.width = bmpInfoHeader.imageWidth;
    bmpPixelInfo.height = bmpInfoHeader.imageHeight;
    bmpPixelInfo.pixelNumber = bmpPixelInfo.width * bmpPixelInfo.height;
    bmpPixelInfo.pixels = new BmpPixel24Bit[bmpPixelInfo.pixelNumber];
    memset(bmpPixelInfo.pixels, 0, bmpPixelInfo.pixelNumber * sizeof(BmpPixel24Bit));

    if (bmpInfoHeader.bitsPerPixel != 24) {
        LOG(ERROR) << "Not 24Bit Image." << std::endl;
        return;
    }

    bmpPixelInfo.bytePerLine = (bmpPixelInfo.width * 3 + 3) / 4 * 4;
    bmpPixelInfo.aligningOffset = bmpPixelInfo.bytePerLine - bmpPixelInfo.width * 3;

    inputFile.seekg(bmpFileHeader.offsetSize, std::ios::beg);
    for (int i = bmpPixelInfo.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpPixelInfo.width; j++) {
            inputFile.read(reinterpret_cast<char *>(bmpPixelInfo.pixels + i * bmpPixelInfo.width + j), sizeof(BmpPixel24Bit));
        }
        inputFile.seekg(bmpPixelInfo.aligningOffset, std::ios::cur);
    }

    inputFile.close();
}

void BmpImage24Bit::toQPixMap(QPixmap &pixmap) {
    const QImage qImage(reinterpret_cast<uchar *>(bmpPixelInfo.pixels) , bmpPixelInfo.width, bmpPixelInfo.height, bmpPixelInfo.width * 3,
                        QImage::Format_BGR888);
    pixmap = QPixmap::fromImage(qImage);
}
