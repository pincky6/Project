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

void PlotScheduler::addTask(QSharedPointer<PlotBuilder> newBuilder)
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

void PlotScheduler::receiveData(QSharedPointer<std::vector<Vertex>> verticies,
                                QSharedPointer<std::vector<unsigned int>> indicies,
                                QString expression)
{
    qDebug() << "scheduler update plot";
    if(!verticies.get() || !indicies.get())
    {
        return;
    }
    emit updatePlot(verticies, indicies, expression);
}
