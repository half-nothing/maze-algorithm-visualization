/**********************************************
 * @file Config.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.23
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>
#include "ConfigDefinition.h"

class Config final : public QObject {
    Q_OBJECT

public:
    static void initConfig();

    static Config *getInstance();

    void saveConfig();

    void readConfig();

    [[nodiscard]] QRgb getConfigField(ConfigField field) const;

    void setConfigField(ConfigField field, const QColor & color);

    Config(const Config &other) = delete;

    Config(Config &&other) noexcept = delete;

    Config &operator=(const Config &other) = delete;

    Config &operator=(Config &&other) noexcept = delete;

signals:
    void configChange();

private:
    Config();

    static Config *instance;
    StoreData storeData;
};

#endif
