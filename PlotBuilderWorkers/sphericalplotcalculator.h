#ifndef SPHERICALPLOTCALCULATOR_H
#define SPHERICALPLOTCALCULATOR_H




#include "abstractplotcalculator.h"

namespace plot_builder
{
using Range = std::array<float, 2>;
using Resolution = std::array<float, 3>;

class SphericalPlotCalculator:  public AbstractPlotCalculator
{
    Q_OBJECT
public:
    SphericalPlotCalculator(const QString&,
                              Range, Range, Range,
                              Resolution, Resolution,
                              QObject* parent = nullptr);
    SphericalPlotCalculator(SphericalPlotCalculator&&) noexcept;
    SphericalPlotCalculator() = delete;
    SphericalPlotCalculator(const SphericalPlotCalculator&) = delete;
    void work() override;
private:
    void calculate();
    float function(float, float);
};
}
#endif // SPHERICALPLOTCALCULATOR_H
