#include "Utils.h"

bool Utils::createDir(const fs::path &path) {
    return fs::create_directories(path);
}

bool Utils::removeDir(const fs::path &path) {
    return fs::remove(path);
}

bool Utils::checkFileExist(const fs::path &path) {
    return fs::exists(path);
}
