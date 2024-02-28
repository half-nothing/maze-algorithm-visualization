#ifndef DAC_BMPIMAGE24BIT_H
#define DAC_BMPIMAGE24BIT_H

#include <string>
#include "BmpImage.hpp"

class BmpImage24Bit : public BmpImage<BmpPixel24Bit> {
public:
    explicit BmpImage24Bit(const std::string &filename);

    void saveImage(const std::string &filename) override;

private:
    void readImage() override;
};

#endif
