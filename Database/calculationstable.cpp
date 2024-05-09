#include "calculationstable.h"

#include <QSqlQuery>
#include <QSqlError>

CalculationResult::CalculationResult()
{}

CalculationResult::CalculationResult(const QString& expression_,
                                     const QString& result_):
    expression(expression_), result(result_)
{}

bool CalculationsTable::create()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS calculationsHistory ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " expression TEXT,"
                    " result TEXT,"
                    " recordId INTEGER NOT NULL,"
                    " FOREIGN KEY (recordId) REFERENCES records(id) ON DELETE CASCADE"
                    ");");
    bool check = query.exec();
    qDebug() << "CREATE: " << query.lastError().text();
    QSqlQuery calculationsHistoryInsertTrigger("CREATE TRIGGER IF NOT EXISTS calculations_history_insert_trigger "
                                               "BEFORE INSERT ON calculationsHistory "
                                               "BEGIN "
                                               "INSERT INTO records (recordTypeId) VALUES (1); "
                                               "END; ");
    calculationsHistoryInsertTrigger.exec();
    qDebug() << "TRIGGER: " << calculationsHistoryInsertTrigger.lastError().text();
    return check;
}

bool CalculationsTable::insert(const CalculationResult& calculationResult)
{
    int index = getIndex();
    QSqlQuery query;
    query.prepare("INSERT INTO calculationsHistory ("
                  " expression,"
                  " result, "
                  " recordId) "
                  "VALUES ("
                  " :expression,"
                  " :result, "
                  " :recordId);");
    query.bindValue(":expression", calculationResult.expression);
    query.bindValue(":result", calculationResult.result);
    query.bindValue(":recordId", index);
    bool check = query.exec();
    qDebug() << "INSERT: " << query.lastError().text();
    return check;
}

bool CalculationsTable::update(const CalculationResult& calculationResult)
{
    QSqlQuery query;
    query.prepare("UPDATE calculationsHistory SET"
                  " result=:result"
                  " WHERE expression=:expression;");
    query.bindValue(":expression", calculationResult.expression);
    query.bindValue(":result", calculationResult.result);
    return query.exec();
}

bool CalculationsTable::remove(const QString& expression)
{
    QSqlQuery query;
    query.prepare("DELETE FROM calculationsHistory WHERE expression = :expression ;");
    query.bindValue(":expression", expression);
    query.exec();
    qDebug() << "REMOVE BY EXPRESSION: " << query.lastError().text();
    return query.next();
}

bool CalculationsTable::existExpression(const QString& expression)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM calculationsHistory WHERE expression = :expression;");
    query.bindValue(":expression", expression);
    query.exec();
    qDebug() << "EXIST EXPRESSION: " << query.lastError().text();
    return query.next();
}
