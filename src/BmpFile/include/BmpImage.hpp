#ifndef DAC_BMPIMAGE_HPP
#define DAC_BMPIMAGE_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <QPixmap>
#include <QPainter>
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
        bmpFile.width = bmpFile.infoHeader.imageWidth;
        bmpFile.height = bmpFile.infoHeader.imageHeight;
        bmpFile.pixelNumber = bmpFile.width * bmpFile.height;
        bmpFile.pixels = new T[bmpFile.pixelNumber];
        vis = new bool[bmpFile.pixelNumber];
        dis = new int32_t[bmpFile.pixelNumber];
        memset(bmpFile.pixels, 0, bmpFile.pixelNumber * sizeof(T));
        memset(vis, 0, bmpFile.pixelNumber * sizeof(bool));
        memset(dis, 0, bmpFile.pixelNumber * sizeof(int32_t));
    };

    ~BmpImage() {
        if (bmpFile.pixels != nullptr) {
            delete[] bmpFile.pixels;
            bmpFile.pixels = nullptr;
        }
        if (bmpFile.palettes != nullptr) {
            delete[] bmpFile.palettes;
            bmpFile.palettes = nullptr;
        }
        if (vis != nullptr) {
            delete[] vis;
            vis = nullptr;
        }
        if (dis != nullptr) {
            delete[] dis;
            dis = nullptr;
        }
        if (inputFile.is_open())
            inputFile.close();
    };

    virtual void readImage() = 0;

    virtual void saveImage(const std::string &filename) = 0;

    virtual void toQPixMap(QPixmap &pixmap) = 0;

    virtual void bfs(QPainter &painter, QPoint start, QPoint end) = 0;

    virtual void dfs(QPainter &painter, QPoint start, QPoint end) = 0;

    virtual void aStar(QPainter &painter, QPoint start, QPoint end) = 0;

protected:
    BmpFile<T> bmpFile;
    std::ifstream inputFile = nullptr;
    bool *vis = nullptr;
    int32_t *dis = nullptr;
};


#endif
