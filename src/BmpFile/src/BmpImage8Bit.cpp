#include "BmpImage8Bit.h"

#include <glog/logging.h>

BmpImage8Bit::BmpImage8Bit() :
    BmpImage() {}

BmpImage8Bit::~BmpImage8Bit() {
    delete [] bmpPixelInfo.pixels;
    delete [] bmpPixelInfo.palettes;
}

void BmpImage8Bit::saveImage(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::ate);
    file.write(reinterpret_cast<char *>(&bmpFileHeader), sizeof(BmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(BmpInfoHeader));
    file.write(reinterpret_cast<char *>(bmpPixelInfo.palettes), sizeof(BmpColorPalette) * bmpPixelInfo.paletteSize);
    for (int i = bmpPixelInfo.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpPixelInfo.width; j++) {
            file.write(reinterpret_cast<char *>(bmpPixelInfo.pixels + bmpPixelInfo.width * i + j), sizeof(uint8_t));
        }
        for (int j = 0; j < bmpPixelInfo.bytePerLine - bmpPixelInfo.width; j++) {
            file.write("0", sizeof(uint8_t));
        }
    }
    file.close();
}

void BmpImage8Bit::readImage(const std::string &filename) {
    BmpImage::readImage(filename);

    bmpPixelInfo.width = bmpInfoHeader.imageWidth;
    bmpPixelInfo.height = bmpInfoHeader.imageHeight;
    bmpPixelInfo.pixelNumber = bmpPixelInfo.width * bmpPixelInfo.height;
    bmpPixelInfo.pixels = new uint8_t[bmpPixelInfo.pixelNumber];
    memset(bmpPixelInfo.pixels, 0, bmpPixelInfo.pixelNumber * sizeof(uint8_t));

    if (bmpInfoHeader.bitsPerPixel != 8) {
        LOG(ERROR) << "Not 8Bit Image." << std::endl;
        return;
    }
    bmpPixelInfo.bytePerLine = (bmpPixelInfo.width + 3) / 4 * 4;
    bmpPixelInfo.aligningOffset = bmpPixelInfo.bytePerLine - bmpPixelInfo.width;

    inputFile.seekg(sizeof(BmpFileHeader) + bmpInfoHeader.headerSize, std::ios::beg);
    if (bmpInfoHeader.colors == 0) {
        bmpPixelInfo.palettes = new BmpColorPalette[256];
        memset(bmpPixelInfo.palettes, 0, sizeof(BmpColorPalette) * 256);
        bmpPixelInfo.paletteSize = 256;
        for (int i = 0; i < 256; i++) {
            inputFile.read(reinterpret_cast<char *>(bmpPixelInfo.palettes + i), sizeof(BmpColorPalette));
        }
    } else {
        bmpPixelInfo.palettes = new BmpColorPalette[bmpInfoHeader.colors];
        memset(bmpPixelInfo.palettes, 0, sizeof(BmpColorPalette) * bmpInfoHeader.colors);
        bmpPixelInfo.paletteSize = bmpInfoHeader.colors;
        for (int i = 0; i < bmpInfoHeader.colors; i++) {
            inputFile.read(reinterpret_cast<char *>(bmpPixelInfo.palettes + i), sizeof(BmpColorPalette));
        }
    }

    inputFile.seekg(bmpFileHeader.offsetSize, std::ios::beg);
    for (int i = bmpPixelInfo.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpPixelInfo.width; j++) {
            inputFile.read(reinterpret_cast<char *>(bmpPixelInfo.pixels + i * bmpPixelInfo.width + j), sizeof(uint8_t));
        }
        inputFile.seekg(bmpPixelInfo.aligningOffset, std::ios::cur);
    }

    inputFile.close();
}

void BmpImage8Bit::toQPixMap(QPixmap &pixmap) {
    const QImage qImage(bmpPixelInfo.pixels, bmpPixelInfo.width, bmpPixelInfo.height, bmpPixelInfo.width,
                        QImage::Format_Indexed8);
    pixmap = QPixmap::fromImage(qImage);
}
