#include "xyzplotcalculator.h"

#include "Marching Cubes/marching_cubes.h"
namespace plot_builder
{
    XYZPlotCalculator::XYZPlotCalculator(const QString& expression,
                                   Range xRange, Range yRange, Range zRange,
                                   Resolution resolution, Resolution screenResolution,
                                   QObject* parent):
        AbstractPlotCalculator(expression, xRange, yRange, zRange,
                               resolution, screenResolution, parent)
    {
        recursiveDescent_.addArgumentToTable("x", firstRange_[0]);
        recursiveDescent_.addArgumentToTable("y", secondRange_[0]);
        recursiveDescent_.addArgumentToTable("z", thirdRange_[0]);
    }

    XYZPlotCalculator::XYZPlotCalculator(XYZPlotCalculator&& calculator) noexcept:
        AbstractPlotCalculator(std::move(calculator))
    {
        recursiveDescent_.addArgumentToTable("x", firstRange_[0]);
        recursiveDescent_.addArgumentToTable("y", secondRange_[0]);
        recursiveDescent_.addArgumentToTable("z", thirdRange_[0]);
    }

    void XYZPlotCalculator::work()
    {
        auto marchingCubes = plot_algorithms::MarchingCubes(expression_, recursiveDescent_);
        QObject::connect(&marchingCubes, &plot_algorithms::MarchingCubes::sendProgress, this, &XYZPlotCalculator::sendProgress);
        auto plot = marchingCubes.triangulate_field(firstRange_, secondRange_, thirdRange_, resolution_, 0.9);
        std::for_each(std::get<0>(plot).begin(), std::get<0>(plot).end(), [this](Vertex& v){
            v.position.setX(v.position.x()/screenResolution_[0]);
            v.position.setY(v.position.y() / screenResolution_[1]);
            v.position.setZ(v.position.z()/ screenResolution_[2]);
        });
        vertices_ = std::move(std::get<0>(plot));
        indices_ = std::move(std::get<1>(plot));
    }
}
