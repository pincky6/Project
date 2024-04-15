#include "plotscheduler.h"
#include <QDebug>
using namespace plot_builder;

PlotScheduler::PlotScheduler(QObject *parent)
    : AbstractPlotWorker{parent}
{
    connect(&(*thread_), &QThread::started, this, &PlotScheduler::whileRun);
    connect(this, &PlotScheduler::finished, thread_.get(), &QThread::quit);
    moveToThread(&(*thread_));
    setRunning(true);
    thread_->start();
}

void PlotScheduler::start()
{
    connect(&(*thread_), &QThread::started, this, &PlotScheduler::whileRun);
    connect(this, &PlotScheduler::finished, thread_.get(), &QThread::quit);
    moveToThread(&(*thread_));
    setRunning(true);
    //thread_->start();
}
void PlotScheduler::work()
{
    if(tasks.size() == 0)
    {
        return;
    }
    qDebug() << "do task";
    tasks.front()->wait();
    qDebug() << "end task";
}

void PlotScheduler::addTask(std::unique_ptr<XYZPlotBuilder>&& newBuilder)
{
    connect(newBuilder.get(), &XYZPlotBuilder::buildingPlotFinish,
            this, &PlotScheduler::receiveData);
    newBuilder->start();
    tasks.push_back(std::move(newBuilder));
}

void PlotScheduler::wait()
{
    stopThread();
    thread_->wait();
}

void PlotScheduler::receiveData(XYZPlotBuilder* readyBuilder,
                                std::vector<Vertex> *verticies,
                                std::vector<unsigned int> *indicies)
{
    if(verticies == nullptr || indicies == nullptr)
    {
        return;
    }
    tasks.remove_if([readyBuilder](auto&& builder){
        return readyBuilder == builder.get();
    });
    emit updatePlot(verticies, indicies);
}
