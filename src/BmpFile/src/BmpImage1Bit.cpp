#include "../include/BmpImage1Bit.h"

BmpImage1Bit::BmpImage1Bit(const std::string &filename) : BmpImage(filename) {
    readImage();
}

void BmpImage1Bit::printImage() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << (bmpFile.pixels[width * i + j] ? '1' : '0');
        }
        std::cout << std::endl;
    }
}

void BmpImage1Bit::saveImage(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::ate);
    file.write(reinterpret_cast<char *>(&bmpFile.fileHeader), sizeof(BmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpFile.infoHeader), sizeof(BmpInfoHeader));
    file.write(reinterpret_cast<char *>(bmpFile.palettes), sizeof(BmpColorPalette) * bmpFile.paletteSize);
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < bytePerLine; j++) {
            uint8_t temp = 0;
            bool flag = false;
            for (int k = j * 8; k < j * 8 + 8; k++) {
                if (k >= width) {
                    flag = true;
                    break;
                }
                temp = temp << 1;
                temp |= bmpFile.pixels[i * width + k] ? 0x01 : 0x00;
            }
            file.write(reinterpret_cast<char *>(&temp), sizeof(uint8_t));
            if (flag)
                break;
        }
        for (int j = 0; j < bytePerLine - (width + 7) / 8 - 1; j++)
            file.write("0", sizeof(uint8_t));
        if (width % 8 != 0)
            file.write("0", sizeof(uint8_t));
    }
    file.close();
}

void BmpImage1Bit::readImage() {
    if (bmpFile.infoHeader.bitsPerPixel != 1) {
        std::cerr << "Not 1Bit Image." << std::endl;
        return;
    }
    bmpFile.palettes = new BmpColorPalette[2]{{0x00, 0x00, 0x00, 0x00},
                                              {0xff, 0xff, 0xff, 0x00}};
    bmpFile.paletteSize = 2;

    inputFile.seekg(bmpFile.fileHeader.offsetSize, std::ios::beg);
    bytePerLine = ((width + 7) / 8 + 3) / 4 * 4;
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < bytePerLine; j++) {
            uint8_t temp;
            bool flag = false;
            inputFile.read(reinterpret_cast<char *>(&temp), sizeof(uint8_t));
            for (int k = j * 8 + 7; k >= j * 8; k--, temp /= 2) {
                if (k >= width) {
                    flag = true;
                    continue;
                }
                bmpFile.pixels[i * width + k] = temp & 1 ? 0xFF : 0x00;
            }
            if (flag)
                break;
        }
        inputFile.seekg(bytePerLine - (width + 7) / 8, std::ios::cur);
        if (width % 8 == 0)
            inputFile.seekg(-1, std::ios::cur);
    }
    inputFile.close();
}

void BmpImage1Bit::toQPixMap(QPixmap &pixmap) {
    QImage qImage((uchar *) bmpFile.pixels, width, height, width, QImage::Format_Indexed8);
    pixmap = QPixmap::fromImage(qImage);
}

void BmpImage1Bit::bfs(QPainter &painter, QPoint start, QPoint end) {
}

void BmpImage1Bit::dfs(QPainter &painter, QPoint start, QPoint end) {

}

void BmpImage1Bit::aStar(QPainter &painter, QPoint start, QPoint end) {

}
