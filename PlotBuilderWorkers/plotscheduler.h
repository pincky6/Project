#ifndef PLOTSCHEDULER_H
#define PLOTSCHEDULER_H

#include <QObject>
#include <QMutex>

#include <list>

#include <QSharedPointer>

#include "PlotBuilderWorkers/plotbuilder.h"

namespace plot_builder
{
    class PlotScheduler : public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        explicit PlotScheduler(QObject *parent = nullptr);
        virtual void work() override;

        void addTask(QSharedPointer<PlotBuilder>);
        void wait() override;
        void start();
        void freeQueue();
    public slots:
        void receiveData(QSharedPointer<std::vector<Vertex>> verticies,
                         QSharedPointer<std::vector<unsigned int>> indices,
                         QString);
    signals:
        void updatePlot(QSharedPointer<std::vector<Vertex>> verticies,
                        QSharedPointer<std::vector<unsigned int>> indices,
                        QString);
    private:
        std::list<QSharedPointer<PlotBuilder>> tasks;
    };
}

#endif // PLOTSCHEDULER_H
