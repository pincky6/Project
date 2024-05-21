#ifndef ABSTRACTPLOTCALCULATOR_H
#define ABSTRACTPLOTCALCULATOR_H

#include "abstractplotworker.h"
#include "Marching Cubes/types.h"
#include "Calculator/recursivedescent.h"

namespace plot_builder
{
using Range = std::array<float, 2>;
using Resolution = std::array<float, 3>;

    class AbstractPlotCalculator: public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        AbstractPlotCalculator(const QString&,
                               Range, Range, Range,
                               Resolution, Resolution,
                               AbstractPlotWorker* parent);
        AbstractPlotCalculator(AbstractPlotCalculator&&) noexcept;
        AbstractPlotCalculator() = delete;
        AbstractPlotCalculator(const AbstractPlotCalculator&) = delete;
        std::vector<Vertex>& getVertices();
        std::vector<unsigned int>& getIndices();

        virtual ~AbstractPlotCalculator(){}
    signals:
        void sendData(std::vector<Vertex>, std::vector<unsigned int>);
        void sendProgress(float);
    protected:
        const QString& expression_;

        Range firstRange_;
        Range secondRange_;
        Range thirdRange_;

        Resolution resolution_;
        Resolution screenResolution_;

        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;

        RecursiveDecsent recursiveDescent;
    };
}

#endif // ABSTRACTPLOTCALCULATOR_H
