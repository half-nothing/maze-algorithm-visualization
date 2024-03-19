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

#include <Definition.h>
#include <QPixmap>
#include <fstream>
#include <string>
#include "BmpStruct.h"

/**
 * @brief Bmp文件操作抽象父类
 */
class BmpImage {
public:
    BmpImage() = default;

    virtual ~BmpImage();

    /**
     * @brief 从指定文件读取图像数据
     * @param[in] filename 要读取的文件名
     */
    virtual void readImage(const std::string &filename);

    /**
     * @brief 保存Bmp文件
     * @param[in] filename Bmp文件保存路径
     */
    virtual void saveImage(const std::string &filename);

    /**
     * @brief 将图像数据转换成Qt的二维向量供QWidget显示
     * @param[out] pixmap Qt二维向量
     */
    virtual void toQPixMap(QPixmap &pixmap);

    BmpType getBmpFileType() const;

protected:
    BmpFileHeader bmpFileHeader{};
    BmpInfoHeader bmpInfoHeader{};
    std::ifstream inputFile = nullptr; //Bmp文件流
};

#endif
