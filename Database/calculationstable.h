#ifndef CALCULATIONSTABLE_H
#define CALCULATIONSTABLE_H

#include "abstracttable.h"
#include <QString>

struct CalculationResult{
public:
    CalculationResult();
    CalculationResult(const QString&,
                      const QString&);
public:
    QString expression;
    QString result;
};

class CalculationsTable: public AbstractTable
{
public:
    bool create() override;
    bool insert(const CalculationResult&);
    bool update(const CalculationResult&);
    bool remove(const QString&);
    bool existExpression(const QString&);
};

#endif // CALCULATIONSTABLE_H
