#include "../include/BmpImage8Bit.h"

BmpImage8Bit::BmpImage8Bit(const std::string &filename) : BmpImage(filename) {
    readImage();
}

void BmpImage8Bit::saveImage(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::out | std::ios::ate);
    file.write(reinterpret_cast<char *>(&bmpFile.fileHeader), sizeof(BmpFileHeader));
    file.write(reinterpret_cast<char *>(&bmpFile.infoHeader), sizeof(BmpInfoHeader));
    file.write(reinterpret_cast<char *>(bmpFile.palettes), sizeof(BmpColorPalette) * bmpFile.paletteSize);
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            file.write(reinterpret_cast<char *>(bmpFile.pixels + width * i + j), sizeof(uint8_t));
        }
        for (int j = 0; j < bytePerLine - width; j++) {
            file.write("0", sizeof(uint8_t));
        }
    }
    file.close();
}

void BmpImage8Bit::readImage() {
    if (bmpFile.infoHeader.bitsPerPixel != 8) {
        std::cerr << "Not 8Bit Image." << std::endl;
        return;
    }
    bytePerLine = (width + 3) / 4 * 4;

    bmpFile.palettes = new BmpColorPalette[256];
    memset(bmpFile.palettes, 0, sizeof(BmpColorPalette) * 256);
    bmpFile.paletteSize = 256;

    inputFile.seekg(bmpFile.fileHeader.offsetSize, std::ios::beg);
    for (int i = 0; i < 256; i++) {
        inputFile.read(reinterpret_cast<char *>(bmpFile.palettes + i), sizeof(BmpColorPalette));
    }

    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            inputFile.read(reinterpret_cast<char *>(bmpFile.pixels + i * width + j), sizeof(uint8_t));
        }
        inputFile.seekg(bytePerLine - width, std::ios::cur);
    }

    inputFile.close();
}
