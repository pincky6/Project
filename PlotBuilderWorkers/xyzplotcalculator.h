#ifndef XYZPLOTCALCULATOR_H
#define XYZPLOTCALCULATOR_H

#include <QObject>
#include <QMutex>

#include "abstractplotcalculator.h"

namespace plot_builder
{
using Range = std::array<float, 2>;
using Resolution = std::array<float, 3>;

    class XYZPlotCalculator:  public AbstractPlotCalculator
    {
        Q_OBJECT
    public:
        XYZPlotCalculator(const QString&,
                       Range, Range, Range,
                       Resolution, Resolution,
                       QObject* parent = nullptr);
        XYZPlotCalculator(XYZPlotCalculator&&) noexcept;
        XYZPlotCalculator() = delete;
        XYZPlotCalculator(const XYZPlotCalculator&) = delete;
        void work() override;
    };
}
#endif // XYZPLOTCALCULATOR_H
