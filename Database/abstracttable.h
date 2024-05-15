#ifndef ABSTRACTTABLE_H
#define ABSTRACTTABLE_H

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
};

#endif // ABSTRACTTABLE_H
