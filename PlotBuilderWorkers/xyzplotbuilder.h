#ifndef XYZPLOTBUILDER_H
#define XYZPLOTBUILDER_H

#include <QVector>
#include <QThread>
#include <QMutex>

#include "Marching Cubes/types.h"
#include "figures.h"

#include "xyzplotcalculator.h"

#include <vector>

namespace plot_builder{

    class XYZPlotBuilder: public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        XYZPlotBuilder();
        XYZPlotBuilder(std::vector<Cube>&, const Resolution&, unsigned int);
        void connect();
        void start();
        void work() override;
        void wait() override;

        void setCubes(std::vector<Cube>&);
        void setThreadsNum(unsigned int);
        void setResolution(const Resolution&);
        void setExpression(const std::vector<QString>&);

        ~XYZPlotBuilder();
    public slots:
        void workFinished();
    signals:
        void buildingPlotFinish(XYZPlotBuilder*,
                                std::shared_ptr<std::vector<Vertex>>, std::shared_ptr<std::vector<unsigned int>>);
    private:
        std::vector<Cube> cubes_;
        Resolution resolution_;
        std::vector<std::unique_ptr<plot_builder::XYZPlotCalculator>> plotCalculators_;
        std::shared_ptr<std::vector<Vertex>> vertices_;
        std::shared_ptr<std::vector<unsigned int>> indices_;
        std::vector<QString> expressionsVector_;


        QMutex mutex_;
        unsigned int threadsCount_;
    };
}

#endif // XYZPLOTBUILDER_H
