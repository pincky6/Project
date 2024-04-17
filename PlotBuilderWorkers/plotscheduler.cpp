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
    tasks.front()->start();
    tasks.front()->wait();
    qDebug() << "end task";
}

void PlotScheduler::addTask(std::unique_ptr<XYZPlotBuilder>&& newBuilder)
{
    QObject::connect(newBuilder.get(), &XYZPlotBuilder::buildingPlotFinish,
            this, &PlotScheduler::receiveData);
    newBuilder->connect();
    tasks.push_back(std::move(newBuilder));
}

void PlotScheduler::wait()
{
    stopThread();
    thread_->wait();
}

void PlotScheduler::receiveData(XYZPlotBuilder* readyBuilder,
                                std::shared_ptr<std::vector<Vertex>> verticies,
                                std::shared_ptr<std::vector<unsigned int>> indicies)
{
    if(verticies.get() == nullptr || indicies.get() == nullptr)
    {
        return;
    }
    tasks.remove_if([readyBuilder](auto&& builder){
        return readyBuilder == builder.get();
    });
    emit updatePlot(verticies, indicies);
}
