#include "recordtable.h"

#include <QSqlQuery>
#include <QSqlError>

RecordTable::RecordTable()
{

}

bool RecordTable::create()
{
    QSqlQuery enableForeignKeyPragma;
    enableForeignKeyPragma.exec("PRAGMA foreign_keys = ON;");
    qDebug() << enableForeignKeyPragma.lastError().text();
    QSqlQuery query("CREATE TABLE IF NOT EXISTS records ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    "recordTypeId INTEGER NOT NULL,"
                    "FOREIGN KEY (recordTypeId) REFERENCES tableTypes(id) ON DELETE CASCADE"
                    ");");
//    ,"
//      "FOREIGN KEY (record_id) REFERENCES plot2D(id) ON DELETE CASCADE,"
//      "FOREIGN KEY (record_id) REFERENCES plot3D(id) ON DELETE CASCADE"
    bool check = query.exec();
    qDebug() << "CREATE RECORD: " << query.lastError().text();
    return check;
}
