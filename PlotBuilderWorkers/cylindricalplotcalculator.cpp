#include "cylindricalplotcalculator.h"

namespace plot_builder
{
    CylindricalPlotCalculator::CylindricalPlotCalculator(const QString& expression,
                                         Range xRange, Range yRange, Range zRange,
                                         Resolution resolution, Resolution screenResolution,
                                         AbstractPlotWorker* parent):
        AbstractPlotCalculator(expression, xRange, yRange, zRange,
                               resolution, screenResolution, parent)
    {
        recursiveDescent.addArgumentToTable("φ", firstRange_[0]);
        recursiveDescent.addArgumentToTable("ρ", secondRange_[0]);
        recursiveDescent.addArgumentToTable("z", thirdRange_[0]);
    }

    CylindricalPlotCalculator::CylindricalPlotCalculator(CylindricalPlotCalculator&& calculator) noexcept:
        AbstractPlotCalculator(std::move(calculator))
    {
        recursiveDescent.addArgumentToTable("φ", firstRange_[0]);
        recursiveDescent.addArgumentToTable("ρ", secondRange_[0]);
        recursiveDescent.addArgumentToTable("z", thirdRange_[0]);
    }

    void CylindricalPlotCalculator::work()
    {
//        auto marchingCubes = plot_algorithms::MarchingCubes(expression_, recursiveDescent);
//        QObject::connect(&marchingCubes, &plot_algorithms::MarchingCubes::sendProgress, this, &CylindricalPlotCalculator::sendProgress);
//        auto plot = marchingCubes.triangulate_field(firstRange_, secondRange_, thirdRange_, resolution_, 0.9);
//        std::for_each(std::get<0>(plot).begin(), std::get<0>(plot).end(), [this](Vertex& v){
//            v.position.setX(v.position.x()/screenResolution_[0]);
//            v.position.setY(v.position.y() / screenResolution_[1]);
//            v.position.setZ(v.position.z()/ screenResolution_[2]);
//        });
//        vertices_ = std::move(std::get<0>(plot));
//        indices_ = std::move(std::get<1>(plot));
//        if(vertices_.size() == 0) return;
    }
}
