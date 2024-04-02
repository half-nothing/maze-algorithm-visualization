#include "BmpImage1Bit.h"

#include <glog/logging.h>

BmpImage1Bit::BmpImage1Bit() :
    BmpImage() {}

BmpImage1Bit::~BmpImage1Bit() {
    delete [] bmpPixelInfo.pixels;
    delete [] bmpPixelInfo.palettes;
}

void BmpImage1Bit::saveImage(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::ate);
    file.write(reinterpret_cast<char *>(&bmpFileHeader), sizeof(BmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(BmpInfoHeader));
    file.write(reinterpret_cast<char *>(bmpPixelInfo.palettes), sizeof(BmpColorPalette) * bmpPixelInfo.paletteSize);
    for (int i = bmpPixelInfo.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpPixelInfo.bytePerLine; j++) {
            uint8_t temp = 0;
            bool flag = false;
            for (int k = j * 8; k < j * 8 + 8; k++) {
                if (k >= bmpPixelInfo.width) {
                    flag = true;
                    break;
                }
                temp = temp << 1;
                temp |= bmpPixelInfo.pixels[i * bmpPixelInfo.width + k] ? 0x01 : 0x00;
            }
            file.write(reinterpret_cast<char *>(&temp), sizeof(uint8_t));
            if (flag) break;
        }
        for (int j = 0; j < bmpPixelInfo.bytePerLine - (bmpPixelInfo.width + 7) / 8 - 1; j++) file.write("0", sizeof(uint8_t));
        if (bmpPixelInfo.width % 8 != 0) file.write("0", sizeof(uint8_t));
    }
    file.close();
}

void BmpImage1Bit::readImage(const std::string &filename) {
    BmpImage::readImage(filename);

    bmpPixelInfo.width = bmpInfoHeader.imageWidth;
    bmpPixelInfo.height = bmpInfoHeader.imageHeight;
    bmpPixelInfo.pixelNumber = bmpPixelInfo.width * bmpPixelInfo.height;
    bmpPixelInfo.pixels = new uint8_t[bmpPixelInfo.pixelNumber];
    memset(bmpPixelInfo.pixels, 0, bmpPixelInfo.pixelNumber * sizeof(uint8_t));

    if (bmpInfoHeader.bitsPerPixel != 1) {
        LOG(ERROR) << "Not 1Bit Image." << std::endl;
        return;
    }

    inputFile.seekg(sizeof(BmpFileHeader) + bmpInfoHeader.headerSize, std::ios::beg);
    bmpPixelInfo.palettes = new BmpColorPalette[2];
    memset(bmpPixelInfo.palettes, 0, sizeof(BmpColorPalette) * bmpInfoHeader.colors);
    bmpPixelInfo.paletteSize = 2;
    for (int i = 0; i < 2; i++) {
        inputFile.read(reinterpret_cast<char *>(bmpPixelInfo.palettes + i), sizeof(BmpColorPalette));
    }

    inputFile.seekg(bmpFileHeader.offsetSize, std::ios::beg);
    bmpPixelInfo.bytePerLine = ((bmpPixelInfo.width + 7) / 8 + 3) / 4 * 4;
    bmpPixelInfo.aligningOffset = bmpPixelInfo.bytePerLine - (bmpPixelInfo.width + 7) / 8;
    for (int i = bmpPixelInfo.height - 1; i >= 0; i--) {
        for (int j = 0; j < bmpPixelInfo.bytePerLine; j++) {
            uint8_t temp;
            bool flag = false;
            inputFile.read(reinterpret_cast<char *>(&temp), sizeof(uint8_t));
            for (int k = j * 8 + 7; k >= j * 8; k--, temp /= 2) {
                if (k >= bmpPixelInfo.width) {
                    flag = true;
                    continue;
                }
                bmpPixelInfo.pixels[i * bmpPixelInfo.width + k] = temp & 1 ? bmpPixelInfo.palettes[1].blue : bmpPixelInfo.palettes[0].blue;
            }
            if (flag) break;
        }
        inputFile.seekg(bmpPixelInfo.aligningOffset, std::ios::cur);
        if (bmpPixelInfo.width % 8 == 0) inputFile.seekg(-1, std::ios::cur);
    }
    inputFile.close();
}

void BmpImage1Bit::toQPixMap(QPixmap &pixmap) {
    const QImage qImage(bmpPixelInfo.pixels, bmpPixelInfo.width, bmpPixelInfo.height, bmpPixelInfo.width, QImage::Format_Indexed8);
    pixmap = QPixmap::fromImage(qImage);
}
