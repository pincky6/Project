#ifndef PLOTSCHEDULER_H
#define PLOTSCHEDULER_H

#include <QObject>
#include <list>

#include "PlotBuilderWorkers/xyzplotbuilder.h"

namespace plot_builder
{
    class PlotScheduler : public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        explicit PlotScheduler(QObject *parent = nullptr);

        virtual void work() override;

        void addTask(std::unique_ptr<XYZPlotBuilder>&&);
        void wait() override;
        void start();
    public slots:
        void receiveData(XYZPlotBuilder* readyBuilder,
                         std::shared_ptr<std::vector<Vertex>> verticies,
                         std::shared_ptr<std::vector<unsigned int>> indices);
    signals:
        void updatePlot(std::shared_ptr<std::vector<Vertex>> verticies,
                        std::shared_ptr<std::vector<unsigned int>> indices);
    private:
        std::list<std::unique_ptr<XYZPlotBuilder>> tasks;
    };
}

#endif // PLOTSCHEDULER_H
