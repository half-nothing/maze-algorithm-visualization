/**********************************************
 * @file BmpFactory.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.20
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef BMPFACTORY_H
#define BMPFACTORY_H
#include <BmpImage.h>

class BmpFactory {
public:
    static BmpImage *createBmpImage(const std::string &filepath);
};

#endif
