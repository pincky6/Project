#ifndef STLMODELSCHEDULER_H
#define STLMODELSCHEDULER_H

#include "abstractplotworker.h"
#include "stlmodelbuilder.h"

namespace plot_builder
{
    class STLModelScheduler: public AbstractPlotWorker
    {
    public:
        STLModelScheduler(QObject *parent = nullptr);

        void work() override;
        void addTask(std::unique_ptr<STLModelBuilder>&&, QWidget* widget = nullptr);
        void wait() override;
        void start();
        void freeQueue();
    private:
        std::list<std::unique_ptr<STLModelBuilder>> tasks;
    };
}
#endif // STLMODELSCHEDULER_H
