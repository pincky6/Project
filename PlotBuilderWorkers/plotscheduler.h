#ifndef PLOTSCHEDULER_H
#define PLOTSCHEDULER_H

#include <QObject>
#include <QMutex>

#include <list>

#include "PlotBuilderWorkers/plotbuilder.h"

namespace plot_builder
{
    class PlotScheduler : public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        explicit PlotScheduler(QObject *parent = nullptr);
        virtual void work() override;

        void addTask(std::unique_ptr<PlotBuilder>&&);
        void wait() override;
        void start();
        void freeQueue();
    public slots:
        void receiveData(std::shared_ptr<std::vector<Vertex>> verticies,
                         std::shared_ptr<std::vector<unsigned int>> indices);
    signals:
        void updatePlot(std::shared_ptr<std::vector<Vertex>> verticies,
                        std::shared_ptr<std::vector<unsigned int>> indices);
    private:
        std::list<std::unique_ptr<PlotBuilder>> tasks;
    };
}

#endif // PLOTSCHEDULER_H
