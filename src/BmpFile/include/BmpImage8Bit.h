#ifndef DAC_BMPIMAGE8BIT_H
#define DAC_BMPIMAGE8BIT_H


#include <string>
#include "BmpImage.hpp"

class BmpImage8Bit : public BmpImage<uint8_t> {
public:
    explicit BmpImage8Bit(const std::string &filename);

    void saveImage(const std::string &filename) override;

private:
    void readImage() override;
};

#endif