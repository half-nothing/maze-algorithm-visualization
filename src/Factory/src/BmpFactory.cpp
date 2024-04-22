/**********************************************
 * @file BmpFactory.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.20
 * @license GNU General Public License (GPL)
 **********************************************/

#include "BmpFactory.h"

#include <BmpImage1Bit.h>
#include <BmpImage24Bit.h>
#include <BmpImage8Bit.h>
#include <QMessageBox>
#include <glog/logging.h>

BmpImage *BmpFactory::createBmpImage(const std::string &filepath) {
    BmpImage bmpImage;
    bmpImage.readImage(filepath);
    switch (bmpImage.getBmpFileType()) {
        case ONE_BIT:
            return new BmpImage1Bit();
        case EIGHT_BIT:
            return new BmpImage8Bit();
        case TWENTY_FOUR_BIT:
            return new BmpImage24Bit();
        case UNKNOWN_BIT:
            QMessageBox::warning(nullptr, "无法加载图片", "不支持的BMP文件格式");
            LOG(ERROR) << "未知的bmp文件格式" << std::endl;
        case LOAD_FAIL:
            return nullptr;
    }
    return nullptr;
}
