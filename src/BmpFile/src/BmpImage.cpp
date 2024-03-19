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

#include <glog/logging.h>

BmpImage::~BmpImage() {
    if (inputFile.is_open()) inputFile.close();
}

void BmpImage::readImage(const std::string &filename) {
    inputFile = std::ifstream(filename, std::ios::binary);
    if (!inputFile) {
        LOG(ERROR) << "Failed to open file: " << filename << std::endl;
        return;
    }
    inputFile.read(reinterpret_cast<char *>(&bmpFileHeader), sizeof(BmpFileHeader));
    if (bmpFileHeader.signature != 0x4D42) {
        LOG(ERROR) << "The file is not a valid BMP file. " << std::endl;
        return;
    }
    inputFile.read(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(BmpInfoHeader));
}

void BmpImage::saveImage(const std::string &filename) {}
void BmpImage::toQPixMap(QPixmap &pixmap) {}

BmpType BmpImage::getBmpFileType() const {
    switch (bmpInfoHeader.bitsPerPixel) {
        case 1: return ONE_BIT;
        case 8: return EIGHT_BIT;
        case 24: return TWENTY_FOUR_BIT;
        default: return UNKNOWN_BIT;
    }
}
