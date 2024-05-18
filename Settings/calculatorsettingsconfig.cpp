#include "calculatorsettingsconfig.h"

#include <QFile>

#include <QMessageBox>

#include <QByteArray>

#include <QJsonObject>
#include <QJsonDocument>

void CalculatorSettingsConfig::exportSettings(const QString& fileName,
                                            const SettingsModel& model)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        throw std::logic_error("Problem with file creation");
    }
    QJsonDocument document(fromSettingsModelToJsonObject(model));
    QByteArray byteArray = document.toJson();
    file.write(byteArray);
    file.close();
}

SettingsModel CalculatorSettingsConfig::importSettings(const QString& fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        throw std::logic_error("Problem with file opening");
    }
    QByteArray byteArray = file.readAll();
    QJsonDocument document(QJsonDocument::fromJson(byteArray));
    if(document.isNull() || !document.isObject())
    {
        throw std::logic_error("Wrong file configuration. Check file");
    }
    SettingsModel model = fromJsonObjectToSettingsModel(document.object());
    file.close();
    return model;
}

QJsonObject CalculatorSettingsConfig::fromSettingsModelToJsonObject(const SettingsModel& model)
{
    QJsonObject object;

    object["resolutionX"] = model.resolutionX;
    object["resolutionY"] = model.resolutionY;
    object["resolutionZ"] = model.resolutionZ;

    object["modelWidth"] = model.modelWidth;
    object["modelLength"] = model.modelLength;
    object["modelHeight"] = model.modelHeight;

    object["model2DRecordsCount"] = model.model2DRecordsCount;
    object["model3DRecordsCount"] = model.model3DRecordsCount;

    return object;
}

SettingsModel CalculatorSettingsConfig::fromJsonObjectToSettingsModel(const QJsonObject& object)
{
    SettingsModel model;

    if(object.constFind("resolutionX") == object.end() ||
       object.constFind("resolutionY") == object.end() ||
       object.constFind("resolutionZ") == object.end() ||
       object.constFind("modelWidth") == object.end() ||
       object.constFind("modelLength") == object.end() ||
       object.constFind("modelHeight") == object.end() ||
       object.constFind("model2DRecordsCount") == object.end() ||
       object.constFind("model3DRecordsCount") == object.end())
    {
        throw std::logic_error("Wrong file configuration. Check file");
    }

    model.resolutionX = object.value("resolutionX").toInt();
    model.resolutionY = object.value("resolutionY").toInt();
    model.resolutionZ = object.value("resolutionX").toInt();

    model.modelWidth = object.value("modelWidth").toInt();
    model.modelLength = object.value("modelLength").toInt();
    model.modelHeight = object.value("modelHeight").toInt();

    model.model2DRecordsCount = object.value("model2DRecordsCount").toInt();
    model.model3DRecordsCount = object.value("model3DRecordsCount").toInt();

    return model;
}
