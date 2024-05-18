#include "settingstable.h"

#include <QSqlQuery>
#include <QSqlError>

bool SettingsTable::create()
{
    SettingsModel model;
    QSqlQuery query("CREATE TABLE IF NOT EXISTS settings ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " resolutionX INTEGER,"
                    " resolutionY INTEGER,"
                    " resolutionZ INTEGER,"
                    " modelWidth  INTEGER,"
                    " modelLength INTEGER,"
                    " modelHeight INTEGER,"
                    " model2DRecordsCount INTEGER,"
                    " model3DRecordsCount INTEGER"
                    ");");
    bool check = query.exec();
    qDebug() << "CREATE: " << query.lastError().text();
    return check && insert(model);
}

bool SettingsTable::insert(const SettingsModel& settingsModel)
{
    QSqlQuery query;
    query.prepare("INSERT INTO settings ("
                  " resolutionX,"
                  " resolutionY,"
                  " resolutionZ,"
                  " modelWidth ,"
                  " modelLength ,"
                  " modelHeight ,"
                  " model2DRecordsCount ,"
                  " model3DRecordsCount)"
                  "VALUES ("
                  " :resolutionX, "
                  " :resolutionY, "
                  " :resolutionZ, "
                  " :modelWidth,"
                  " :modelLength,"
                  " :modelHeight,"
                  " :model2DRecordsCount,"
                  " :model3DRecordsCount);");
    query.bindValue(":resolutionX", settingsModel.resolutionX);
    query.bindValue(":resolutionY", settingsModel.resolutionY);
    query.bindValue(":resolutionZ", settingsModel.resolutionZ);
    query.bindValue(":modelWidth", settingsModel.modelWidth);
    query.bindValue(":modelLength", settingsModel.modelLength);
    query.bindValue(":modelHeight", settingsModel.modelHeight);
    query.bindValue(":model2DRecordsCount", settingsModel.model2DRecordsCount);
    query.bindValue(":model3DRecordsCount", settingsModel.model3DRecordsCount);

    bool check = query.exec();
    qDebug() << "INSERT: " << query.lastError().text();
    return check;
}

bool SettingsTable::update(const SettingsModel& settingsModel)
{
    QSqlQuery query;
    query.prepare("UPDATE settings SET"
                  " resolutionX = :resolutionX,"
                  " resolutionY = :resolutionY,"
                  " resolutionZ = :resolutionZ,"
                  " modelWidth  = :modelWidth,"
                  " modelLength = :modelLength,"
                  " modelHeight = :modelHeight,"
                  " model2DRecordsCount := :model2DRecordsCount ,"
                  " model3DRecordsCount := :model3DRecordsCount "
                  " WHERE id = 1;");
    query.bindValue(":resolutionX", settingsModel.resolutionX);
    query.bindValue(":resolutionY", settingsModel.resolutionY);
    query.bindValue(":resolutionZ", settingsModel.resolutionZ);
    query.bindValue(":modelWidth", settingsModel.modelWidth);
    query.bindValue(":modelLength", settingsModel.modelLength);
    query.bindValue(":modelHeight", settingsModel.modelHeight);
    query.bindValue(":model2DRecordsCount", settingsModel.model2DRecordsCount);
    query.bindValue(":model3DRecordsCount", settingsModel.model3DRecordsCount);
    bool check = query.exec();
    qDebug() << "UPDATE: " << query.lastError().text();
    return check;
}

bool SettingsTable::removeByExpression(const QString& expression)
{
    Q_UNUSED(expression)
    QSqlQuery query;
    query.prepare("DELETE FROM settings WHERE id = 1;");
    query.exec();
    qDebug() << "REMOVE BY EXPRESSION: " << query.lastError().text();
    return query.next();
}
