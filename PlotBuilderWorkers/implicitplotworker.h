#ifndef IMPLICITPLOTWORKER_H
#define IMPLICITPLOTWORKER_H

#include "Marching Squares/marchingsquares.h"
#include "abstractplotworker.h"
#include <QPainter>
#include <QMutex>

class ImplicitPlotWorker: public AbstractPlotWorker
{
public:
    ImplicitPlotWorker(const QString& expression, marching_squares::EdgeList& edgeList,
                          float widthStart, float widthEnd,
                          float heightStart, float heightEnd,
                          float resolutionX, float resolutionY,
                          int width, int height,
                          QMutex& mutex, QPainter& painter,
                          AbstractPlotWorker* parent = nullptr);
    void work() override;
    ~ImplicitPlotWorker(){}
private:
    const QString& expression_;
    marching_squares::EdgeList& edgeList_;

    float widthStart_;
    float widthEnd_;

    float heightStart_;
    float heightEnd_;

    float resolutionX_;
    float resolutionY_;

    int width_;
    int height_;

    QMutex& mutex_;

    QPainter& painter_;
};

#endif
