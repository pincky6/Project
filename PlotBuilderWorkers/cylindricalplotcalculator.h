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
                      QObject* parent = nullptr);
    CylindricalPlotCalculator(CylindricalPlotCalculator&&) noexcept;
    CylindricalPlotCalculator() = delete;
    CylindricalPlotCalculator(const CylindricalPlotCalculator&) = delete;
    void work() override;
private:
    void calulate();
    float function(float fi, float ro, float z);
};
}

#endif // CYLINDRICALPLOTCALCULATOR_H
