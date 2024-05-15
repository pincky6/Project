#ifndef RECORDTYPE_H
#define RECORDTYPE_H

#include <QString>

namespace record::type
{
enum class RecordType
{
    EXPRESSION,
    PLOT2D,
    PLOT3D
};
QString toStringRepresentation(RecordType);
RecordType fromStringRepresentation(const QString& typeString);
}



#endif // RECORDTYPE_H
