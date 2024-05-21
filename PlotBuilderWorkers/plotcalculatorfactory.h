#ifndef PLOTCALCULATORFACTORY_H
#define PLOTCALCULATORFACTORY_H

#include "xyzplotcalculator.h"
#include "cylindricalplotcalculator.h"
#include "sphericalplotcalculator.h"

namespace plot_builder
{
    class PlotCalculatorFactory
    {
    public:
        static AbstractPlotCalculator* produce(const QString& expression,
                                                             Range firstRange, Range secondRange, Range thirdRange,
                                                             Resolution resolution, Resolution screenResolution, QObject* parent = nullptr)
        {
            auto mathCharacters = containsCharacters(expression);
            if(mathCharacters.contains("x") && mathCharacters.contains("y") && mathCharacters.contains("z"))
            {
                return new XYZPlotCalculator(expression,
                                             firstRange, secondRange, thirdRange,
                                             resolution, screenResolution, parent);
            }
            else if(mathCharacters.contains("θ") && mathCharacters.contains("φ"))
            {
                return new SphericalPlotCalculator(expression,
                                                   firstRange, secondRange, thirdRange,
                                                   resolution, screenResolution, parent);
            }
            else if(mathCharacters.contains("φ") || mathCharacters.contains("z"))
                                                     {
                                                         return new CylindricalPlotCalculator(expression,
                                                                                              firstRange, secondRange, thirdRange,
                                                                                              resolution, screenResolution, parent);
                                                     }
            return nullptr;
        }
    private:
        static QVector<QString> containsCharacters(const QString& expression)
        {
            const QVector<QString> mathCharacters = {"x", "y", "z", "φ", "θ", "ρ"};
            QVector<QString> foundCharacters;
            foreach(auto&& mathCharacter, mathCharacters)
            {
                if(expression.contains(mathCharacter))
                {
                    foundCharacters.push_back(mathCharacter);
                }
            }
            return foundCharacters;
        }
    };
}

#endif // PLOTCALCULATORFACTORY_H
