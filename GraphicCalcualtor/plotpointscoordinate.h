#ifndef PLOTPOINTSCOORDINATE_H
#define PLOTPOINTSCOORDINATE_H
#include <vector>
#include <memory>
#include <QPointF>

enum class PlotPointsCoordinateType
{
    Explicit, Polar, Implicit
};



class ExplicitPlotPointsCoordinate
{
public:
    void getPointsForPlot(std::vector<QPointF>&, const QString&,int, int);
};

class PolarPlotPointsCoordinate
{
public:
    void getPointsForPlot(std::vector<QPointF>&, const QString&, int, int);
};

class ImplicitPlotPointsCoordinate
{
public: 
    void getPointsForPlot(std::vector<std::array<std::array<double, 2>, 2>>&, const QString&, float, float, float, float, int, int);
};
#endif // PLOTPOINTSCOORDINATE_H
