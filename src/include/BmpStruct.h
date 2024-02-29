#ifndef DAC_BMPSTRUCT_H
#define DAC_BMPSTRUCT_H


#include <cstdint>
;
#pragma pack(push, 1)
// 按1字节压栈对齐
struct BmpFileHeader {
    // 文件头
    uint16_t signature;  // 文件头,两个字节,0x4D42
    uint32_t fileSize;   // 文件大小,四个字节
    uint32_t reserved;   // 保留位,必须为0,四个字节
    uint32_t offsetSize; // 数据偏移量,四个字节
    // 14 字节
};

struct BmpInfoHeader {
    // 位图信息头
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
    // 40 字节
};

// 调色板信息
struct BmpColorPalette {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
};

struct BmpPixel24Bit {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};

#pragma pack(pop)

template<typename T>
struct BmpFile {
    BmpFileHeader fileHeader{};
    BmpInfoHeader infoHeader{};
    BmpColorPalette *palettes = nullptr;
    T *pixels = nullptr;
    uint32_t paletteSize = 0;
};

#endif
