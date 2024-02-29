/**********************************************
 * @file Utils.h
 * @brief 工具类头文件
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.2.29
 * @license GNU General Public License (GPL)
 **********************************************/
#ifndef DAC_UTILS_H
#define DAC_UTILS_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class Utils {
public:
    static bool createDir(const fs::path &path);

    static bool removeDir(const fs::path &path);

    static bool checkFileExist(const fs::path &path);
};

#endif
