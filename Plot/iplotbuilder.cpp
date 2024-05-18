#include "iplotbuilder.h"
#include "plotpointscoordinate.h"
#include "PlotBuilderWorkers/implicitplotworker.h"

#include <memory>
#include <QDebug>
#include <thread>
#include <QVector>

#include <Database/plottable2d.h>

std::unique_ptr<IPlotBuilder> IPlotBuilder::makePlotBuilder(PlotType plotType)
{
    switch (plotType)
    {
    case PlotType::Explicit:
        return std::unique_ptr<IPlotBuilder>(new ExplicitPlotBuilder);
    case PlotType::Polar:
        return std::unique_ptr<IPlotBuilder>(new PolarPlotBuilder);
    case PlotType::Implicit:
        return std::unique_ptr<IPlotBuilder>(new ImplicitPlotBuilder);
    default:
        static_assert ("error");
    }
    throw std::exception();
}

void ExplicitPlotBuilder::draw(QPainter& painter, const QString& expression, int width, int height)
{
    std::vector<QPointF> vectorOfPoints;
    ExplicitPlotPointsCoordinate plotCoordinate;
    float maxDiff = 0.35;
    plotCoordinate.getPointsForPlot(vectorOfPoints, expression, width, height);
    for(std::size_t i = 1; i < vectorOfPoints.size(); i++)
    {
        if(fabs(vectorOfPoints[i].x() - vectorOfPoints[i - 1].x()) <= maxDiff)
            painter.drawLine(vectorOfPoints[i - 1], vectorOfPoints[i]);
    }
}

void PolarPlotBuilder::draw(QPainter& painter, const QString& expression, int width, int height)
{
    std::vector<QPointF> vectorOfPoints;
    PolarPlotPointsCoordinate  plotCoordiante;
    plotCoordiante.getPointsForPlot(vectorOfPoints, expression, width, height);

    for(std::size_t i = 1; i < vectorOfPoints.size(); i++)
    {

        painter.drawLine(vectorOfPoints[i - 1], vectorOfPoints[i]);
    }
}


void ImplicitPlotBuilder::divideDisplay(float width, float height, float top, float left, int n, std::vector<Rectangle>& result)
{
    if(n == 0) return;
    if(n == 1)
    {
        result.push_back(Rectangle(width, height, top, left));
        return;
    }
    double l = n / 2.0;
    double r = n - l;

    double newWidth = width;
    double newHeight = height;

    if(width > height)
    {
        newWidth /= 2.0;
        divideDisplay(newWidth, newHeight, top, left, l, result);
        divideDisplay(newWidth, newHeight, top, left + newWidth, r, result);
    }
    else
    {
        newHeight /= 2.0;

        divideDisplay(newWidth, newHeight, top, left, l, result);
        divideDisplay(newWidth, newHeight, top + newHeight, left, r, result);
    }
}

void ImplicitPlotBuilder::draw(QPainter& painter, const QString& expression, int width, int height)
{
    PlotTable2D table;
    if(table.existExpression(expression))
    {
        Plot2D plot2D = table.selectByExpression(expression);
        for(auto&& point: *plot2D.vertices)
        {
            painter.drawPoint((float)width/2.0 + point[0][0] *40.0, height/2.0 - (float)point[0][1] * 40.0);
            painter.drawPoint((float)width/2.0 + point[1][0] *40.0, height/2.0 - (float)point[1][1] * 40.0);
        }
        return;
    }
    std::vector<std::unique_ptr<ImplicitPlotWorker>> threadPool;
    marching_squares::EdgeList* edgeList = new marching_squares::EdgeList();
    std::vector<Rectangle> rectangleVector;
    int countOfRect = (std::thread::hardware_concurrency() - 1) == 0 ? 1 : (std::thread::hardware_concurrency() - 1);
    divideDisplay(width, height, 0, 0, countOfRect, rectangleVector);
    for(auto&& rectangle: rectangleVector)
    {
        float wStart = (rectangle.Left - width/2.0)/40.0;
        float wEnd = wEnd = (rectangle.Left + rectangle.Width - width/2.0)/40.0;
        float hStart = (rectangle.Top - height/2.0)/40.0;
        float hEnd = hEnd = (rectangle.Top + rectangle.Height - height/2.0)/40.0;
        ImplicitPlotWorker* worker = new ImplicitPlotWorker(expression, *edgeList, wStart, wEnd, hStart, hEnd,
                                                                  rectangle.Width, rectangle.Height, width, height, mut, painter);
        threadPool.emplace_back(worker);
    }
    for(auto&& thread: threadPool)
    {
        thread.get()->wait();
    }
    Plot2D plot2D(expression, edgeList);
    table.insertOrUpdate(plot2D);
}



