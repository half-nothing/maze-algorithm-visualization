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
