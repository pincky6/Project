#include "typetable.h"

#include <QSqlQuery>
#include <QSqlError>

TypeTable::TypeTable()
{

}

bool TypeTable::create()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS tablesType ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " type TEXT"
                    ");");
    query.exec();
    if(!existExpression("CalculationsHistory"))
    {
        insert("CalculationsHistory");
    }
    if(!existExpression("Plot2D"))
    {
        insert("Plot2D");
    }
    if(!existExpression("Plot3D"))
    {
        insert("Plot3D");
    }
    return query.next();
}

bool TypeTable::insert(const QString& type)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tablesType(type) VALUES(:type);");

    query.bindValue(":type", type);
    qDebug() << "INSERT: " << query.lastError().text();
    return query.exec();
}


bool TypeTable::existExpression(const QString& type)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM tablesType WHERE type = :type;");
    query.bindValue(":type", type);
    query.exec();
    qDebug() << "EXIST EXPRESSION: " << query.lastError().text();
    return query.next();
}
