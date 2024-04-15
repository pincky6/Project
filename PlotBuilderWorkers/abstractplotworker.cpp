#include "abstractplotworker.h"

AbstractPlotWorker::AbstractPlotWorker(QObject* parent ):
    QObject(parent), flag_(true), thread_(new QThread(parent))
{

}

void AbstractPlotWorker::setRunning(bool newRunning)
{
    if (flag_ == newRunning)
        return;
    flag_ = newRunning;
}

void AbstractPlotWorker::wait()
{
    emit finished();
    while(thread_->isRunning());
}

void AbstractPlotWorker::quit()
{
    thread_->quit();
}

void AbstractPlotWorker::stopThread()
{
    setRunning(false);
    wait();
}

AbstractPlotWorker::~AbstractPlotWorker()
{
    stopThread();
}

void AbstractPlotWorker::whileRun()
{
    while(flag_)
    {
        work();
        thread_->sleep(1);
    }
    emit finished();
}

void AbstractPlotWorker::run()
{
    work();
    emit finished();
}
