#ifndef ABSTRACTPLOTAREA_H
#define ABSTRACTPLOTAREA_H

#include <vector>
#include <QString>

class AbstractPlotArea
{
public:
    AbstractPlotArea();
    virtual void setExpressions(const std::vector<QString>&) = 0;
    virtual ~AbstractPlotArea();
};

#endif // ABSTRACTPLOTAREA_H
