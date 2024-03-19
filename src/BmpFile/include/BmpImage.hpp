/**********************************************
 * @file BmpImage.hpp
 * @brief Bmp文件操作的抽象父类
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.3.3
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_BMPIMAGE_HPP
#define DAC_BMPIMAGE_HPP

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <QPixmap>
#include "BmpStruct.h"

/**
 * @brief Bmp文件操作抽象父类
 * @tparam T 像素点的类型名{@link BmpFile}
 */
template<typename T = uint8_t>
class BmpImage {
public:
    BmpImage() = default;

    virtual ~BmpImage() {
        delete[] bmpFile.pixels;
        delete[] bmpFile.palettes;
        if (inputFile.is_open()) inputFile.close();
    }

    /**
     * @brief 从指定文件读取图像数据
     * @param[in] filename 要读取的文件名
     */
    virtual void readImage(const std::string &filename) {
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
        memset(bmpFile.pixels, 0, bmpFile.pixelNumber * sizeof(T));
    }

    /**
     * @brief 保存Bmp文件
     * @param[in] filename Bmp文件保存路径
     */
    virtual void saveImage(const std::string &filename) = 0;

    /**
     * @brief 将图像数据转换成Qt的二维向量供QWidget显示
     * @param[out] pixmap Qt二维向量
     */
    virtual void toQPixMap(QPixmap &pixmap) = 0;

protected:
    BmpFile<T> bmpFile{};              //Bmp文件数据
    std::ifstream inputFile = nullptr; //Bmp文件流
};

#endif
