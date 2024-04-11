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

        void start();
        void work() override;
        void wait() override;
        void clearBuffers();

        void setCubes(std::vector<Cube>&);
        void setThreadsNum(unsigned int);
        void setResolution(const Resolution&);
        void setExpression(const std::vector<QString>&);

        std::vector<Vertex>& getVertices();
        std::vector<unsigned int>& getIndices();

        ~XYZPlotBuilder();
    public slots:
        void workFinished();
    signals:
        void buildingPlotFinish();
    private:
        std::vector<Cube> cubes_;
        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;
        Resolution resolution_;
        std::vector<std::unique_ptr<plot_builder::XYZPlotCalculator>> plotCalculators_;

        std::vector<QString> expressionsVector_;


        QMutex mutex_;
        unsigned int threadsCount_;
    };
}

#endif // XYZPLOTBUILDER_H
