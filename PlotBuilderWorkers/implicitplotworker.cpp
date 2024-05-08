#include "implicitplotworker.h"
#include "Marching Squares/marchingsquares.h"
#include "Plot/plotpointscoordinate.h"

#include <QDebug>

ImplicitPlotWorker::ImplicitPlotWorker(const QString& expression, marching_squares::EdgeList& edgeList, float widthStart, float widthEnd,
                                             float heightStart, float heightEnd, float resolutionX, float resolutionY, int width, int height,
                                             QMutex& mutex, QPainter& painter,
                                             AbstractPlotWorker* parent):
    AbstractPlotWorker(parent), expression_(expression), edgeList_(edgeList), widthStart_(widthStart), widthEnd_(widthEnd), heightStart_(heightStart),
    heightEnd_(heightEnd), resolutionX_(resolutionX), resolutionY_(resolutionY), width_(width), height_(height), mutex_(mutex), painter_(painter)
{

    connect(&(*thread_), &QThread::started, this, &ImplicitPlotWorker::run);
    connect(this, &ImplicitPlotWorker::finished, &(*thread_), &QThread::quit);
    moveToThread(&(*thread_));
    thread_->start();
}

void ImplicitPlotWorker::work()
{
    marching_squares::EdgeList vectorOfPoints;
    ImplicitPlotPointsCoordinate  plotCoordiante;
    plotCoordiante.getPointsForPlot(vectorOfPoints, expression_, edgeList_, widthStart_,
                                    widthEnd_, heightStart_, heightEnd_, resolutionX_, resolutionY_);
    mutex_.lock();
    std::copy(vectorOfPoints.begin(), vectorOfPoints.end(), std::back_inserter(edgeList_));
    for(auto&& point: vectorOfPoints)
    {
            painter_.drawPoint((float)width_/2.0 + point[0][0] *40.0, height_/2.0 - (float)point[0][1] * 40.0);
            painter_.drawPoint((float)width_/2.0 + point[1][0] *40.0, height_/2.0 - (float)point[1][1] * 40.0);
    }
    mutex_.unlock();
}
