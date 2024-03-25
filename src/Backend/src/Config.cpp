/**********************************************
 * @file Config.cpp
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.23
 * @license GNU General Public License (GPL)
 **********************************************/

#include "Config.h"
#include <fstream>

#include <qdir.h>
#include <glog/logging.h>

Config *Config::instance = nullptr;
static const QDir configPath = QDir::cleanPath(QDir::currentPath() + QDir::separator() + "config");
static const QDir configFilePath = QDir::cleanPath(configPath.absolutePath() + QDir::separator() + "config.dat");

Config::Config() {
    LOG(INFO) << "Config path: " << configPath.absolutePath().toStdString();
    if (!configPath.exists()) {
        configPath.mkpath(configPath.absolutePath());
    }
    LOG(INFO) << "Config file path: " << configFilePath.absolutePath().toStdString();
    connect(this, &Config::configChange, &Config::saveConfig);
    readConfig();
}

void Config::initConfig() {
    if (instance == nullptr) {
        instance = new Config();
        return;
    }
    LOG(ERROR) << "Function initConfig can only be called once";
}

Config *Config::getInstance() {
    if (instance == nullptr) {
        LOG(FATAL) << "Function initConfig must be called before this function is called!";
        return nullptr;
    }
    return instance;
}

void Config::saveConfig() {
    LOG(INFO) << "Save Config";
    std::fstream output(configFilePath.filesystemPath(), std::ios::out | std::ios::binary);
    if (output.is_open()) {
        for (int i = 0; i < STORE_DATA_LEN; i++) {
            output.write(rc_char_p(storeData.colorConfig + i), sizeof(ColorStore));
        }
    }
    output.close();
}

void Config::readConfig() {
    LOG(INFO) << "Read Config";
    std::fstream input(configFilePath.filesystemPath(), std::ios::in | std::ios::binary);
    if (!input.is_open()) {
        saveConfig();
        input.close();
        return;
    }
    for (int i = 0; i < STORE_DATA_LEN; i++) {
        input.read(rc_char_p(storeData.colorConfig + i), sizeof(ColorStore));
        if (input.peek() == EOF) {
            break;
        }
    }
    input.close();
}

QRgb Config::getConfigField(const ConfigField field) const {
    return qRgb(storeData.colorConfig[field].red,
                storeData.colorConfig[field].green,
                storeData.colorConfig[field].blue);
}

void Config::setConfigField(const ConfigField field, const QColor &color) {
    storeData.colorConfig[field].red = color.red();
    storeData.colorConfig[field].green = color.green();
    storeData.colorConfig[field].blue = color.blue();
    emit configChange();
}
