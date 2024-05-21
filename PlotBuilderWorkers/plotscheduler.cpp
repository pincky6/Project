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

}

void PlotScheduler::start()
{
    thread_->start();
}

void PlotScheduler::freeQueue()
{
    tasks.clear();
}
void PlotScheduler::work()
{
    if(tasks.size() == 0)
    {
        return;
    }
    auto task = std::move(tasks.front());
    tasks.pop_front();
    task->start();
    task->wait();
}

void PlotScheduler::addTask(std::unique_ptr<PlotBuilder>&& newBuilder)
{
    QObject::connect(newBuilder.get(), &PlotBuilder::buildingPlotFinish,
            this, &PlotScheduler::receiveData);
    newBuilder->connect();
    tasks.push_back(std::move(newBuilder));
}

void PlotScheduler::wait()
{
    stopThread();
    thread_->wait();
}

void PlotScheduler::receiveData(std::shared_ptr<std::vector<Vertex>> verticies,
                                std::shared_ptr<std::vector<unsigned int>> indicies)
{
    if(verticies.get() == nullptr || indicies.get() == nullptr)
    {
        return;
    }
    emit updatePlot(verticies, indicies);
}
