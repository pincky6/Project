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
    if(!task->getVertices() || !task->getIndices())
    {
        return;
    }
    emit updatePlot(task->getVertices(), task->getIndices(), task->getExpression());
}

void PlotScheduler::addTask(QSharedPointer<PlotBuilder> newBuilder)
{
    newBuilder->connect();
    tasks.push_back(std::move(newBuilder));
}

void PlotScheduler::wait()
{
    stopThread();
    thread_->wait();
}
