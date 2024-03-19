/**********************************************
* @file BmpImage24Bit.h
 * @brief 24位位图文件操作类头文件
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.3.3
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_BMPIMAGE24BIT_H
#define DAC_BMPIMAGE24BIT_H

#include <string>
#include "BmpImage.hpp"

/**
 * @class BmpImage24Bit
 * @brief 24位位图文件操作类
 */
class BmpImage24Bit final : public BmpImage<BmpPixel24Bit> {
public:
    BmpImage24Bit();

    void readImage(const std::string &filename) override;

    void saveImage(const std::string &filename) override;

    void toQPixMap(QPixmap &pixmap) override;
};

#endif
