#include "typetable.h"

#include <QSqlQuery>
#include <QSqlError>

TypeTable::TypeTable()
{

}

bool TypeTable::create()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS tableTypes ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " type TEXT"
                    ");");
    bool check = query.exec();
    qDebug() << "CREATE TYPES: "<< query.lastError().text();
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
    return check;
}

bool TypeTable::removeByExpression(const QString& type)
{
    QSqlQuery query;
    query.prepare("DELETE FROM tableTypes WHERE type = :type;");
    query.bindValue(":type", type);
    query.exec();
    qDebug() << "REMOVE BY EXPRESSION: " << query.lastError().text();
    return query.next();
}

bool TypeTable::insert(const QString& type)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tableTypes(type) VALUES(:type);");

    query.bindValue(":type", type);
    qDebug() << "INSERT: " << query.lastError().text();
    return query.exec();
}


bool TypeTable::existExpression(const QString& type)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM tableTypes WHERE type = :type;");
    query.bindValue(":type", type);
    query.exec();
    qDebug() << "EXIST EXPRESSION: " << query.lastError().text();
    return query.next();
}
