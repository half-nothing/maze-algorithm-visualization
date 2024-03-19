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

#include <cstdint>
#include "BmpImage.hpp"

/**
 * @class BmpImage1Bit
 * @brief 单位位图文件操作类
 */
class BmpImage1Bit final : public BmpImage<uint8_t> {
public:
    BmpImage1Bit();

    void readImage(const std::string &filename) override;

    void saveImage(const std::string &filename) override;

    void toQPixMap(QPixmap &pixmap) override;
};

#endif
