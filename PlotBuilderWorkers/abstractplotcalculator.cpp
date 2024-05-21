#include "abstractplotcalculator.h"

namespace plot_builder
{

    AbstractPlotCalculator::AbstractPlotCalculator(const QString& expression,
                                         Range firstRange, Range secondRange, Range thirdRange,
                                         Resolution resolution, Resolution screenResolution,
                                         AbstractPlotWorker* parent):
        AbstractPlotWorker(parent), expression_(expression),
        firstRange_(firstRange), secondRange_(secondRange), thirdRange_(thirdRange),
        resolution_(resolution), screenResolution_(screenResolution)
    {

        connect(&(*thread_.get()), &QThread::started, this, &AbstractPlotCalculator::run);
        connect(this, &AbstractPlotCalculator::finished, &(*thread_.get()), &QThread::quit);
        moveToThread(&(*thread_.get()));
        thread_->start();
    }

    AbstractPlotCalculator::AbstractPlotCalculator(AbstractPlotCalculator&& calculator) noexcept:
        AbstractPlotWorker(calculator.parent()), expression_(std::move(calculator.expression_)),
        firstRange_(std::move(calculator.firstRange_)),
        secondRange_(std::move(calculator.secondRange_)),
        thirdRange_(std::move(calculator.thirdRange_)),
        resolution_(std::move(calculator.resolution_)), screenResolution_(std::move(calculator.screenResolution_))
    {
    }

    std::vector<Vertex> &AbstractPlotCalculator::getVertices()
    {
        return vertices_;
    }

    std::vector<unsigned int> &AbstractPlotCalculator::getIndices()
    {
        return indices_;
    }
}
