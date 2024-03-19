/**********************************************
 * @file BmpImage1Bit.h
 * @brief 单位位图文件操作类头文件
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.3.3
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_BMPIMAGE1BIT_H
#define DAC_BMPIMAGE1BIT_H

#include "BmpImage.h"

/**
 * @class BmpImage1Bit
 * @brief 单位位图文件操作类
 */
class BmpImage1Bit final : public BmpImage {
public:
    BmpImage1Bit();

    ~BmpImage1Bit() override;

    void readImage(const std::string &filename) override;

    void saveImage(const std::string &filename) override;

    void toQPixMap(QPixmap &pixmap) override;

private:
    BmpPixelInfo<uint8_t> bmpPixelInfo{};
};

#endif
