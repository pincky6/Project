#ifndef IMPLICITWORKERBUILDER_H
#define IMPLICITWORKERBUILDER_H

#include "abstractplotbuilder.h"
#include <QPainter>
#include <QMutex>

class ImplicitWorkerBuilder: public AbstractPlotBuilder
{
public:
    ImplicitWorkerBuilder(const QString& expression, float widthStart, float widthEnd,
                          float heightStart, float heightEnd, float resolutionX, float resolutionY, int width, int height,
                            QMutex& mutex, QPainter& painter,
                          AbstractPlotBuilder* parent = nullptr);
//    ImplicitWorkerBuilder(ImplicitWorkerBuilder&& anotherWorker): expression_(anotherWorker.expression_), widthStart_(anotherWorker.widthStart_), widthEnd_(anotherWorker.widthEnd_),
//        heightStart_(anotherWorker.heightStart_),heightEnd_(anotherWorker.heightEnd_), resolutionX_(anotherWorker.resolutionX_), resolutionY_(anotherWorker.resolutionY_),
//        width_(anotherWorker.width_), height_(anotherWorker.height_)
//    {

//    }
    void work() override;
    ~ImplicitWorkerBuilder(){}
private:
    const QString& expression_;
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

#endif // IMPLICITWORKERBUILDER_H
