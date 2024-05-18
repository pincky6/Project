#ifndef ABSTRACTTABLE_H
#define ABSTRACTTABLE_H

#include <QString>

#include <QSqlQuery>
#include <QSqlError>

class AbstractTable
{
public:
    virtual bool create() = 0;
    virtual bool removeByExpression(const QString&) = 0;
    int getIndex()
    {
        QSqlQuery indexQuery("SELECT seq FROM sqlite_sequence WHERE name = \"records\";");
        indexQuery.exec();
        int index = 1;
        if(indexQuery.first())
        {
            int rowId = indexQuery.value(0).toInt();
            index = (rowId == 0) ? 1: (rowId + 1);
        }
        return index;
    }
    virtual ~AbstractTable(){}
protected:
    int getRowCount(const QString& tableName)
    {
        QSqlQuery getRowCountQuery;
        getRowCountQuery.prepare(QString("SELECT COUNT(*) FROM %1 ;").arg(tableName));
        getRowCountQuery.exec();
        if(getRowCountQuery.next())
        {
            int rowCount = getRowCountQuery.value(0).toInt();
            return rowCount;
        }
        else
        {
            return -1;
        }

    }
    int getMaxId(const QString& tableName)
    {
        QSqlQuery getMaxQuery;
        getMaxQuery.prepare(QString("SELECT MAX(id) FROM %1 ;").arg(tableName));
        getMaxQuery.exec();
        if(getMaxQuery.next())
        {
            int maxId = getMaxQuery.value(0).toInt();
            return maxId;
        }
        else
        {
            return -1;
        }

    }
    bool drop(const QString& tableName, unsigned int rowCount)
    {
        QString value = QString("DELETE FROM %1 "
                                "WHERE id IN "
                                "(SELECT id FROM %2 "
                                "ORDER BY id DESC LIMIT %3);")
                            .arg(tableName)
                            .arg(tableName)
                            .arg(rowCount);
        QSqlQuery query(value);
        bool check = query.exec();
        qDebug() << "DROP: " << query.lastError().text();
        return check;
    }

};

#endif // ABSTRACTTABLE_H
