/**********************************************
 * @file ConfigStruct.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.23
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef CONFIGSTRUCT_H
#define CONFIGSTRUCT_H
#include <Definition.h>
#include <cstdint>

struct ColorStore {
    uint8_t red{};
    uint8_t green{};
    uint8_t blue{};

    ColorStore() = default;

    ColorStore(const uint8_t red, const uint8_t green, const uint8_t blue) :
        red{red},
        green{green},
        blue{blue} {}

    explicit ColorStore(const QColor &color) {
        red = color.red();
        green = color.green();
        blue = color.blue();
    }

    ColorStore(const ColorStore &other) = default;

    ColorStore(ColorStore &&other) noexcept :
        red{other.red},
        green{other.green},
        blue{other.blue} {}

    ColorStore &operator=(const ColorStore &other) = default;

    ColorStore &operator=(ColorStore &&other) noexcept {
        if (this == &other) return *this;
        blue = other.blue;
        green = other.green;
        red = other.red;
        return *this;
    }
};

struct StoreData {
    /**
     * 0    开始节点颜色
     * 1    结束节点颜色
     * 2    墙壁颜色
     * 3    已搜索像素颜色
     * 4    搜索到的路径颜色
     */
    ColorStore colorConfig[STORE_DATA_LEN]{
            {0, 255, 0},
            {255, 0, 0},
            {0, 0, 0},
            {0, 0, 255},
            {255, 255, 0}};
};

enum ConfigField {
    START_POINT_COLOR,
    END_POINT_COLOR,
    WALL_COLOR,
    SEARCHED_POINT_COLOR,
    PATH_POINT_COLOR
};

#endif
