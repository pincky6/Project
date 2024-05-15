#ifndef TABLEFACTORY_H
#define TABLEFACTORY_H

#include "calculationstable.h"
#include "plottable2d.h"
#include "plottable3d.h"
#include "recordtable.h"

class TableFactory
{
public:
    static std::shared_ptr<AbstractTable> produce(record::type::RecordType type)
    {
        switch (type) {

        case record::type::RecordType::EXPRESSION:
            return std::shared_ptr<AbstractTable>(new CalculationsTable());
        case record::type::RecordType::PLOT2D:
            return std::shared_ptr<AbstractTable>(new PlotTable2D());
        case record::type::RecordType::PLOT3D:
            return std::shared_ptr<AbstractTable>(new PlotTable3D());
            break;
        }
        return nullptr;
    }
};

#endif // TABLEFACTORY_H
