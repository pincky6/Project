#ifndef XYZPLOTCALCULATOR_H
#define XYZPLOTCALCULATOR_H

#include <QObject>
#include <QMutex>

#include "Marching Cubes/types.h"
#include "Calculator/recursivedescent.h"

#include "abstractplotworker.h"

namespace plot_builder
{
using Range = std::array<float, 2>;
using Resolution = std::array<float, 3>;

    class XYZPlotCalculator:  public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        XYZPlotCalculator(const QString& expression,
                       Range xRange, Range yRange, Range zRange,
                       Resolution resolution, Resolution screenResolution,
                       AbstractPlotWorker* parent = nullptr);
        XYZPlotCalculator() = delete;
        XYZPlotCalculator(const XYZPlotCalculator&) = delete;
        XYZPlotCalculator(XYZPlotCalculator&&) noexcept;
        void work() override;
        std::vector<Vertex>& getVertices();
        std::vector<unsigned int>& getIndices();
        ~XYZPlotCalculator(){}
    signals:
        void sendData(std::vector<Vertex>, std::vector<unsigned int>);
    private:
        const QString& expression_;

        Range xRange_;
        Range yRange_;
        Range zRange_;

        Resolution resolution_;
        Resolution screenResolution_;

        std::vector<Vertex> vertices_;
        std::vector<unsigned int> indices_;

        RecursiveDecsent recursiveDescent;
    };
}
#endif // XYZPLOTCALCULATOR_H
