#ifndef DAC_BMPIMAGE_HPP
#define DAC_BMPIMAGE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <QPixmap>
#include "BmpStruct.h"

template<typename T>
class BmpImage {
public:
    explicit BmpImage(const std::string &filename) {
        inputFile = std::ifstream(filename, std::ios::binary);
        if (!inputFile) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }
        inputFile.read(reinterpret_cast<char *>(&bmpFile.fileHeader), sizeof(BmpFileHeader));
        if (bmpFile.fileHeader.signature != 0x4D42) {
            std::cerr << "The file is not a valid BMP file. " << std::endl;
            return;
        }

        inputFile.read(reinterpret_cast<char *>(&bmpFile.infoHeader), sizeof(BmpInfoHeader));
        width = bmpFile.infoHeader.imageWidth;
        height = bmpFile.infoHeader.imageHeight;
        bmpFile.pixels = new T[width * height];
        memset(bmpFile.pixels, 0, width * height * sizeof(T));
    };

    ~BmpImage() {
        if (bmpFile.pixels != nullptr)
            delete[] bmpFile.pixels;
        if (bmpFile.palettes != nullptr)
            delete[] bmpFile.palettes;
        if (inputFile.is_open())
            inputFile.close();
    };

    virtual void readImage() = 0;

    virtual void saveImage(const std::string &filename) = 0;

    virtual void toQPixMap(QPixmap &pixmap) = 0;

protected:
    BmpFile<T> bmpFile;
    std::ifstream inputFile = nullptr;
    uint32_t width;
    uint32_t height;
    uint32_t bytePerLine{};
};


#endif
