#include "iplotbuilder.h"
#include "plotpointscoordinate.h"
#include "PlotBuilderWorkers/implicitworkerbuilder.h"

#include <memory>
#include <QDebug>
#include <thread>
#include <QVector>

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

    plotCoordinate.getPointsForPlot(vectorOfPoints, expression, width, height);
    for(std::size_t i = 1; i < vectorOfPoints.size(); i++)
    {
        if(fabs(vectorOfPoints[i].x() - vectorOfPoints[i - 1].x()) <= 0.35)
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


void ImplicitPlotBuilder::divideDisplay(float width, float height, float top, float left, int n, std::vector<Rect>& result)
{
    if(n == 0) return;
    if(n == 1)
    {
        result.push_back(Rect(width, height, top, left));
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
    std::vector<std::unique_ptr<ImplicitWorkerBuilder>> threadPool;
    std::vector<Rect> rectangleVector;
    int countOfRect = (std::thread::hardware_concurrency() - 1) == 0 ? 1 : (std::thread::hardware_concurrency() - 1);
    divideDisplay(width, height, 0, 0, countOfRect, rectangleVector);
    qDebug() << rectangleVector.size();
    for(auto&& rectangle: rectangleVector)
    {
        qDebug() << std::thread::hardware_concurrency();
        float wStart = (rectangle.Left - width/2.0)/40.0, wEnd = (rectangle.Left + rectangle.Width - width/2.0)/40.0;
        float hStart = (rectangle.Top - height/2.0)/40.0, hEnd = (rectangle.Top + rectangle.Height - height/2.0)/40.0;
        qDebug() << width << height;
//        threadPool.emplace_back(&ImplicitPlotBuilder::drawHelper, this, /*std::ref(painter),*/ std::cref(expression),
//                                wStart, wEnd, hStart, hEnd, rectangle.Width, rectangle.Height, width, height);
       //drawHelper(/*painter,*/ expression, wStart, wEnd, hStart, hEnd, rectangle.Width, rectangle.Height, width, height);
        ImplicitWorkerBuilder* worker = new ImplicitWorkerBuilder(expression, wStart, wEnd, hStart, hEnd,
                                                                  rectangle.Width, rectangle.Height, width, height, mut, painter);
        threadPool.emplace_back(worker);
    }

    for(auto&& thread: threadPool)
    {
        thread.get()->wait();
        qDebug() << "end";
    }

}

void ImplicitPlotBuilder::drawHelper(/*QPainter& painter, */const QString& expression, float widthStart, float widthEnd,
                                     float heightStart, float heightEnd, float resolutionX, float resolutionY, int width, int height)
{
    std::vector<std::array<std::array<double, 2>, 2>> vectorOfPoints;
    ImplicitPlotPointsCoordinate  plotCoordiante;
    plotCoordiante.getPointsForPlot(vectorOfPoints, expression, widthStart, widthEnd, heightStart, heightEnd, resolutionX, resolutionY);
    qDebug() << "end";
//    for(auto&& point: vectorOfPoints)
//    {
//        painter.drawPoint((float)width/2.0 + point[0][0] *40.0, height/2.0 - (float)point[0][1] * 40.0);
//        painter.drawPoint((float)width/2.0 + point[1][0] *40.0, height/2.0 - (float)point[1][1] * 40.0);
//    }
}



