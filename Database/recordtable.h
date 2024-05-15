#ifndef RECORDTABLE_H
#define RECORDTABLE_H

#include "abstracttable.h"
#include "Database/recordtype.h"

struct Record
{
    Record(const QString&,
           const record::type::RecordType&);
    Record();
public:
    QString expression;
    record::type::RecordType type;
};

class RecordTable: AbstractTable
{
public:
    RecordTable();
    bool create() override;
    bool removeByExpression(const QString&) override;
    QVector<Record> selectAll();
};

#endif // RECORDTABLE_H
