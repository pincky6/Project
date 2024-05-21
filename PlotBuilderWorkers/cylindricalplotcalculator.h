#ifndef CYLINDRICALPLOTCALCULATOR_H
#define CYLINDRICALPLOTCALCULATOR_H


#include "abstractplotcalculator.h"

namespace plot_builder
{
using Range = std::array<float, 2>;
using Resolution = std::array<float, 3>;

class CylindricalPlotCalculator:  public AbstractPlotCalculator
{
    Q_OBJECT
public:
    CylindricalPlotCalculator(const QString&,
                      Range, Range, Range,
                      Resolution, Resolution,
                      AbstractPlotWorker* parent = nullptr);
    CylindricalPlotCalculator(CylindricalPlotCalculator&&) noexcept;
    CylindricalPlotCalculator() = delete;
    CylindricalPlotCalculator(const CylindricalPlotCalculator&) = delete;
    void work() override;
};
}

#endif // CYLINDRICALPLOTCALCULATOR_H
