#ifndef PLOTBUILDER_H
#define PLOTBUILDER_H

#include <QVector>
#include <QThread>
#include <QMutex>

#include "Marching Cubes/types.h"
#include "figures.h"

#include <QSharedPointer>

#include <vector>

#include "abstractplotcalculator.h"

namespace plot_builder{

    class PlotBuilder: public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        PlotBuilder();
        PlotBuilder(std::vector<Cube>&, const Resolution&, unsigned int);
        void connect();
        void start();
        void work() override;
        void wait() override;

        void setCubes(std::vector<Cube>&);
        void setThreadsNum(unsigned int);
        void setResolution(const Resolution&);
        void setExpression(const QString&);

        QSharedPointer<std::vector<Vertex>> getVertices();
        QSharedPointer<std::vector<unsigned int>> getIndices();
        QString getExpression();

        ~PlotBuilder();
    public slots:
        void workFinished();
    signals:
        void buildingPlotFinish(QSharedPointer<std::vector<Vertex>>,
                                QSharedPointer<std::vector<unsigned int>>,
                                QString);
    private:
        std::vector<Cube> cubes_;
        Resolution resolution_;
        std::vector<std::unique_ptr<plot_builder::AbstractPlotCalculator>> plotCalculators_;
        QSharedPointer<std::vector<Vertex>> vertices_;
        QSharedPointer<std::vector<unsigned int>> indices_;
        QString expression_;


        QMutex mutex_;
        unsigned int threadsCount_;
    };
}

#endif // PLOTBUILDER_H
