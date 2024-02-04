#include "implicitworkerbuilder.h"
#include "Plot/plotpointscoordinate.h"

#include <QDebug>

ImplicitWorkerBuilder::ImplicitWorkerBuilder(const QString& expression, float widthStart, float widthEnd,
                                             float heightStart, float heightEnd, float resolutionX, float resolutionY, int width, int height,
                                             QMutex& mutex, QPainter& painter,
                                             AbstractPlotBuilder* parent):
    AbstractPlotBuilder(parent), expression_(expression), widthStart_(widthStart), widthEnd_(widthEnd), heightStart_(heightStart),
    heightEnd_(heightEnd), resolutionX_(resolutionX), resolutionY_(resolutionY), width_(width), height_(height), mutex_(mutex), painter_(painter)
{

    connect(&thread, &QThread::started, this, &ImplicitWorkerBuilder::run);
    connect(this, &ImplicitWorkerBuilder::finished, &thread, &QThread::quit);
    moveToThread(&thread);
    thread.start();
}

void ImplicitWorkerBuilder::work()
{
    std::vector<std::array<std::array<double, 2>, 2>> vectorOfPoints;
    ImplicitPlotPointsCoordinate  plotCoordiante;
    plotCoordiante.getPointsForPlot(vectorOfPoints, expression_, widthStart_,
                                    widthEnd_, heightStart_, heightEnd_, resolutionX_, resolutionY_);

    mutex_.lock();
    for(auto&& point: vectorOfPoints)
    {
            painter_.drawPoint((float)width_/2.0 + point[0][0] *40.0, height_/2.0 - (float)point[0][1] * 40.0);
            painter_.drawPoint((float)width_/2.0 + point[1][0] *40.0, height_/2.0 - (float)point[1][1] * 40.0);
    }
    mutex_.unlock();
}
