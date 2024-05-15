#include "recordtable.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QVector>


using namespace record::type;

Record::Record(const QString& expression_,
               const RecordType& type_):
    expression(expression_),
    type(type_)
{

}

RecordTable::RecordTable()
{

}

QString record::type::toStringRepresentation(RecordType type)
{
    switch(type)
    {
    case record::type::RecordType::EXPRESSION:
        return "EXPRESSION";
    case record::type::RecordType::PLOT2D:
        return "PLOT2D";
    case record::type::RecordType::PLOT3D:
        return "PLOT3D";
    default:
        return "";
    }
}

record::type::RecordType record::type::fromStringRepresentation(const QString& typeString)
{
    if(typeString == "EXPRESSION")
    {
        return RecordType::EXPRESSION;
    }
    else if(typeString == "PLOT2D")
    {
        return RecordType::PLOT2D;
    }
    else
    {
        return RecordType::PLOT3D;
    }
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
    bool check = query.exec();
    qDebug() << "CREATE RECORD: " << query.lastError().text();
    return check;
}

bool RecordTable::removeByExpression(const QString& recordTypeId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM records WHERE recordTypeId = :recordTypeId;");
    query.bindValue(":recordTypeId", recordTypeId);
    query.exec();
    qDebug() << "REMOVE BY EXPRESSION: " << query.lastError().text();
    return query.next();
}

QVector<Record> RecordTable::selectAll()
{
    QVector<Record> records;
    QSqlQuery query;
    query.exec("SELECT expression, r.recordTypeId "
               "FROM ("
               "SELECT expression, recordId FROM calculationsHistory "
               "UNION ALL "
               "SELECT expression, recordId  FROM plot2D pd "
               "UNION ALL "
               "SELECT expression, recordId  FROM plot3D "
               ") AS subquery "
               "JOIN records r ON subquery.recordId = r.id "
               "ORDER BY recordId;");
    while(query.next())
    {
        records.push_back(Record(query.value("expression").toString(),
                                 (record::type::RecordType)(query.value("recordTypeId").toInt() - 1)));
    }
    return records;
}
