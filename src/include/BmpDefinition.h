/**********************************************
 * @file BmpDefinition.h
 * @brief Bmp文件信息结构体
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_BMPSTRUCT_H
#define DAC_BMPSTRUCT_H

#include <cstdint>
;
#pragma pack(push, 1)
// 按1字节压栈对齐

/**
 * @struct BmpFileHeader
 * @brief Bmp文件头结构体
 */
struct BmpFileHeader {
    uint16_t signature;  // 位图文件头,两个字节,0x4D42
    uint32_t fileSize;   // 文件大小,四个字节
    uint32_t reserved;   // 保留位,必须为0,四个字节
    uint32_t offsetSize; // 数据偏移量,四个字节
};

/**
 * @struct BmpInfoHeader
 * @brief Bmp信息头结构体
 */
struct BmpInfoHeader {
    uint32_t headerSize;      // 信息头大小,四个字节,一般为40
    uint32_t imageWidth;      // 图像宽度,四个字节
    uint32_t imageHeight;     // 图像高度,四个字节
    uint16_t planes;          // 画布,两个字节,值为0x0001
    uint16_t bitsPerPixel;    // 色深,两个字节
    uint32_t compression;     // 是否压缩,四个字节
    uint32_t imageSize;       // 图像大小,四个字节
    uint32_t widthPPM;        // 水平分辨率,四个字节
    uint32_t heightPPM;       // 垂直分辨率,四个字节
    uint32_t colors;          // 使用的颜色索引数,四个字节
    uint32_t importantColors; // 主要颜色索引数,四个字节
};

/**
 * @struct BmpColorPalette
 * @brief 调色板信息结构体
 */
struct BmpColorPalette {
    uint8_t blue;     // 蓝色分量
    uint8_t green;    // 绿色分量
    uint8_t red;      // 红色分量
    uint8_t reserved; // 保留位
};

/**
 * @struct BmpPixel24Bit
 * @brief 24bit像素信息结构体
 */
struct BmpPixel24Bit {
    uint8_t blue;  // 蓝色分量
    uint8_t green; // 绿色分量
    uint8_t red;   // 红色分量
};

#pragma pack(pop)

/**
 * @struct BmpPixelInfo
 * @brief BMP文件像素结构体
 * @tparam T 像素点的类型
 * @par 像素点类型:
 *      单色位图和8位位图都是uint8_t \n
 *      24位位图是BmpPixel24Bit
 */
template<typename T>
struct BmpPixelInfo {
    BmpColorPalette *palettes = nullptr; /// 调色板数组
    T *pixels = nullptr;                 // 像素点数组
    uint32_t width = 0;                  // 图片宽度
    uint32_t height = 0;                 // 图片高度
    uint32_t bytePerLine = 0;            // 每行像素数量
    uint32_t pixelNumber = 0;            // 总像素量
    uint32_t aligningOffset = 0;         // 4K对齐偏移量
    /**
     * @brief 调色板的数量 \n
     *      一般情况下\n
     *      单色位图=2 \n
     *      8位位图=256 \n
     *      24位位图没有调色板
     **/
    uint32_t paletteSize = 0;
};

enum BmpType {
    LOAD_FAIL = -1,
    UNKNOWN_BIT = -2,
    ONE_BIT = 1,
    EIGHT_BIT = 8,
    TWENTY_FOUR_BIT = 24
};

#endif
