#ifndef ABSTRACTTABLE_H
#define ABSTRACTTABLE_H

class AbstractTable
{
public:
    virtual bool create() = 0;
    virtual ~AbstractTable(){}
};

#endif // ABSTRACTTABLE_H
