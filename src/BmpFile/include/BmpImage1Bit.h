#ifndef DAC_BMPIMAGE1BIT_H
#define DAC_BMPIMAGE1BIT_H

#include <cstdint>
#include "BmpImage.hpp"

class BmpImage1Bit : public BmpImage<uint8_t>
{
public:
    explicit BmpImage1Bit(const std::string &filename);

    void printImage();

    void saveImage(const std::string &filename) override;

private:
    void readImage() override;
};

#endif