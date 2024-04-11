#include "xyzplotcalculator.h"

#include "Marching Cubes/marching_cubes.h"
namespace plot_builder
{
    XYZPlotCalculator::XYZPlotCalculator(const QString& expression,
                                   Range xRange, Range yRange, Range zRange,
                                   Resolution resolution, Resolution screenResolution,
                                   AbstractPlotWorker* parent):
        AbstractPlotWorker(parent), expression_(expression),
        xRange_(xRange), yRange_(yRange), zRange_(zRange),
        resolution_(resolution), screenResolution_(screenResolution)
    {

        connect(&(*thread_.get()), &QThread::started, this, &XYZPlotCalculator::run);
        connect(this, &XYZPlotCalculator::finished, &(*thread_.get()), &QThread::quit);
        moveToThread(&(*thread_.get()));
        thread_->start();

        recursiveDescent.addArgumentToTable("x", xRange_[0]);
        recursiveDescent.addArgumentToTable("y", yRange_[0]);
        recursiveDescent.addArgumentToTable("z", zRange_[0]);
    }

    XYZPlotCalculator::XYZPlotCalculator(XYZPlotCalculator&& calculator) noexcept:
        AbstractPlotWorker(calculator.parent()), expression_(std::move(calculator.expression_)),
        xRange_(std::move(calculator.xRange_)), yRange_(std::move(calculator.yRange_)), zRange_(std::move(calculator.zRange_)),
        resolution_(std::move(calculator.resolution_)), screenResolution_(std::move(calculator.screenResolution_))
    {
        recursiveDescent.addArgumentToTable("x", xRange_[0]);
        recursiveDescent.addArgumentToTable("y", yRange_[0]);
        recursiveDescent.addArgumentToTable("z", zRange_[0]);
    }

    void XYZPlotCalculator::work()
    {
        auto plot = plot_algorithms::MarchingCubes(expression_, recursiveDescent).triangulate_field(xRange_, yRange_, zRange_, resolution_, 0.9);
        std::for_each(std::get<0>(plot).begin(), std::get<0>(plot).end(), [this](Vertex& v){
            v.position.setX(v.position.x()/screenResolution_[0]);
            v.position.setY(v.position.y() / screenResolution_[1]);
            v.position.setZ(v.position.z()/ screenResolution_[2]);
        });
        vertices_ = std::move(std::get<0>(plot));
        indices_ = std::move(std::get<1>(plot));
        if(vertices_.size() == 0) return;
        auto minmax = std::minmax_element(vertices_.cbegin(), vertices_.cend(),
                                          [](auto&& lhs, auto&& rhs){
                                              return lhs.position.z() < rhs.position.z();
                                          });

        double min = minmax.first->position.z();
        double max = minmax.second->position.z();

        double yellow_bound_min = min/2.0f + (max + min) / 2.0f;
        double yellow_bound_max = max/ 2.0f + (max + min) / 2.0f;

        double red_bound_min = min / 4.0f + yellow_bound_min;
        double red_bound_max = max / 4.0f + yellow_bound_max;

        std::for_each(vertices_.begin(), vertices_.end(), [&](Vertex& p){
            if((p.position.z() <= yellow_bound_min && p.position.z() >= red_bound_min)||
                (p.position.z() >= yellow_bound_max && p.position.z() <= red_bound_max))
            {
                p.color = QVector3D(1.0f, 1.0f, 0.0f);
            }
            else if(p.position.z() <= red_bound_min || p.position.z() >= red_bound_max)
            {
                p.color = QVector3D(1.0f, 0.0f, 0.0f);
            }
            else
            {
                p.color = QVector3D(0.0, 1.0f, 0.0f);
            }
        });

    }

    std::vector<Vertex> &XYZPlotCalculator::getVertices()
    {
        return vertices_;
    }

    std::vector<unsigned int> &XYZPlotCalculator::getIndices()
    {
        return indices_;
    }
}
