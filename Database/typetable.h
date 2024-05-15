#ifndef TYPETABLE_H
#define TYPETABLE_H

#include "abstracttable.h"

#include <QString>

class TypeTable : public AbstractTable
{
public:
    TypeTable();
    bool create() override;
    bool removeByExpression(const QString&) override;
    bool insert(const QString&);
    bool existExpression(const QString&);
};

#endif // TYPETABLE_H
