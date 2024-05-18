#ifndef CALCULATORSETTINGSCONFIG_H
#define CALCULATORSETTINGSCONFIG_H

#include <QString>
#include "Settings/settingsmodel.h"

class QJsonObject;

class CalculatorSettingsConfig
{
public:
    static void exportSettings(const QString&, const SettingsModel&);
    static SettingsModel importSettings(const QString&);

    static QJsonObject fromSettingsModelToJsonObject(const SettingsModel&);
    static SettingsModel fromJsonObjectToSettingsModel(const QJsonObject&);
};

#endif // CALCULATORSETTINGSCONFIG_H
