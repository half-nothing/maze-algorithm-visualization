/**********************************************
 * @file BmpImage.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.20
 * @license GNU General Public License (GPL)
 **********************************************/

#include "BmpImage.h"

#include <QMessageBox>
#include <glog/logging.h>

BmpImage::~BmpImage() {
    if (inputFile.is_open()) inputFile.close();
}

void BmpImage::readImage(const std::string &filename) {
    inputFile = std::ifstream(filename, std::ios::binary);
    if (!inputFile) {
        LOG(ERROR) << "Failed to open file: " << filename << std::endl;
        QMessageBox::warning(nullptr, "无法打开图片", QString::asprintf("无法打开文件: %s", filename.c_str()));
        return;
    }
    inputFile.read(reinterpret_cast<char *>(&bmpFileHeader), sizeof(BmpFileHeader));
    if (bmpFileHeader.signature != 0x4D42) {
        LOG(ERROR) << "The file is not a valid BMP file. " << std::endl;
        QMessageBox::warning(nullptr, "无法打开图片", "此文件不是有效的BMP文件");
        return;
    }
    inputFile.read(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(BmpInfoHeader));
    imageLoaded = true;
}

void BmpImage::saveImage(const std::string &filename) {}
void BmpImage::toQPixMap(QPixmap &pixmap) {}

BmpType BmpImage::getBmpFileType() const {
    if (!this->imageLoaded) {
        return LOAD_FAIL;
    }
    switch (bmpInfoHeader.bitsPerPixel) {
        case 1: return ONE_BIT;
        case 8: return EIGHT_BIT;
        case 24: return TWENTY_FOUR_BIT;
        default:
            LOG(ERROR) << QString::asprintf("不支持的色深度：%d位", bmpInfoHeader.bitsPerPixel).toStdString();
            return UNKNOWN_BIT;
    }
}
