#ifndef RECORDTABLE_H
#define RECORDTABLE_H

#include "abstracttable.h"

class RecordTable: AbstractTable
{
public:
    RecordTable();
    bool create() override;
    bool remove();
    bool exists();
};

#endif // RECORDTABLE_H
