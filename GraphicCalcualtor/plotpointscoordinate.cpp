#include "plotpointscoordinate.h"
#include "Calculator/recursivedescent.h"
#include <vector>
#include <QDebug>
#include "marchingsquares.h"


void ExplicitPlotPointsCoordinate::getPointsForPlot(std::vector<QPointF>& pointsVector, const QString& expression,int width, int height)
{
    float step = 0.015625/2.0, x = 0;

    RecursiveDecsent rec;
    rec.addArgumentToTable("x", Double(x));
    rec.setEpsilon(0.007);

    Operand& op = rec.getElementFromArgumentTable("x");
    Double y;
    while(x <= width / 80.0)
    {
        try
        {
            y = dynamic_cast<Double&>(*rec.calculate(expression).getOperand());
            pointsVector.emplace_back(width/2.0 + 40.0*x, height/2.0 - 40.0*(double)y);
        }
        catch(const DomainError& error)
        {
            pointsVector.emplace_back(width/2.0 + 40.0*(x + step), height/2.0 - error.value);
            x += 2 * step;
            op +=  (Double)(2*step);
        }
        catch(...)
        {}
        x += step;
        op += (Double)step;
    }
}

void PolarPlotPointsCoordinate::getPointsForPlot(std::vector<QPointF> &pointsVector, const QString &expression, int width, int height)
{

    float endOfCalculation = 2*(width/80.0 + height/80.0)*M_PI;
    float  grad = 0;
    float step = 0.125/4.0;

    RecursiveDecsent rec;
    rec.addArgumentToTable("φ", Double(grad));
    rec.setEpsilon(0.007);

    Operand& op = rec.getElementFromArgumentTable("φ");

    Double x = 0, y;

    QString xExpression = "(" + expression + ")*cos(φ)", yExpression = "(" + expression + ")*sin(φ)";
    while(grad < endOfCalculation )
    {
        try
        {
            x = dynamic_cast<Double&>(*rec.calculate(xExpression).getOperand());
            y = dynamic_cast<Double&>(*rec.calculate(yExpression).getOperand());
            pointsVector.emplace_back(width/2.0 + 40.0*(double)x, height/2.0 - 40.0*(double)y);
        }
        catch(const DomainError& error)
        {
            grad += 2 * step;
            op += (Double)step;
        }
        catch (...) {}
        grad += step;
        op += (Double)step;
    }
}


void ImplicitPlotPointsCoordinate::getPointsForPlot(
        std::vector<std::array<std::array<double, 2>, 2>>& pointsVector,
        const QString &expression,
        float widthStart,float widthEnd,
        float heightStart, float heightEnd,
        int width, int height)
{

    RecursiveDecsent rec;
    rec.setEpsilon(0.01);
    rec.addArgumentToTable("x", widthStart);
    rec.addArgumentToTable("y", heightStart);

    RecursiveDecsent recursiveDecsent;
    const auto mc = marching_squares::MarchingSquares(expression, {(double)widthStart, (double)widthEnd},
                                                      {(double)heightStart, (double)heightEnd},
                                                      {width, height});
    pointsVector = mc.compute(0.9, recursiveDecsent);

}
