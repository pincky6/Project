#include "abstractplotbuilder.h"
#include <QDebug>

AbstractPlotBuilder::AbstractPlotBuilder(QObject* parent ):
    QObject(parent), flag(true)
{

}

void AbstractPlotBuilder::setRunning(bool newRunning)
{
    if (flag == newRunning)
        return;
    flag = newRunning;
}

void AbstractPlotBuilder::wait()
{
    emit finished();
    while(thread.isRunning());
}

void AbstractPlotBuilder::stopThread()
{
    setRunning(false);
    wait();
}

AbstractPlotBuilder::~AbstractPlotBuilder()
{
    stopThread();
}

void AbstractPlotBuilder::whileRun()
{
    while(flag)
    {
        work();
        thread.sleep(5);
    }
    emit finished();
}

void AbstractPlotBuilder::run()
{
    work();
    emit finished();
}
