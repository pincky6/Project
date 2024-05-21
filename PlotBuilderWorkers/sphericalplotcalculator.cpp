#include "sphericalplotcalculator.h"

namespace plot_builder
{

    SphericalPlotCalculator::SphericalPlotCalculator(const QString& expression,
                                                     Range fiRange, Range thetaRange, Range zRange,
                                                     Resolution resolution, Resolution screenResolution, QObject *parent):
        AbstractPlotCalculator(expression,
                               fiRange, thetaRange, zRange,
                               resolution, screenResolution, parent)
    {

    //    "φ", "ρ", "θ"
        recursiveDescent_.addArgumentToTable("θ", firstRange_[0]);
        recursiveDescent_.addArgumentToTable("φ", secondRange_[0]);
    }

    SphericalPlotCalculator::SphericalPlotCalculator(SphericalPlotCalculator&& spherialCalculator) noexcept:
        AbstractPlotCalculator(std::move(spherialCalculator))
    {
        recursiveDescent_.addArgumentToTable("θ", firstRange_[0]);
        recursiveDescent_.addArgumentToTable("φ", secondRange_[0]);
    }

    void SphericalPlotCalculator::work()
    {
        calculate();
    }

    void SphericalPlotCalculator::calculate()
    {
        if(firstRange_[0] > firstRange_[1]) std::swap(firstRange_[0], firstRange_[1]);
        if(secondRange_[0] > secondRange_[1]) std::swap(secondRange_[0], secondRange_[1]);


        float dFi = float(firstRange_[1] - firstRange_[0]) / (float)resolution_[0];
        float dTheta = float(secondRange_[1] - secondRange_[0]) / (float)resolution_[1];


        float xLen = fabs(firstRange_[0]) + fabs(firstRange_[1]);
        float yLen = fabs(secondRange_[0]) + fabs(secondRange_[1]);
        float zLen = fabs(thirdRange_[0]) + fabs(thirdRange_[1]);

        float fiLower = firstRange_[0];
        float thetaLower = secondRange_[0];

        for(int i = 0; i < resolution_[0]; i++)
        {
            float thetaAngle = thetaLower + float(i) * dTheta;
            for (int j = 0; j < resolution_[1]; j++)
            {
                float fiAngle = fiLower + float(j)* dFi;


                float res = function(thetaAngle, fiAngle);

                float x = res * sin(thetaAngle) * cos(fiAngle) / resolution_[0];
                float y = res * sin(thetaAngle) * sin(fiAngle) / resolution_[1];
                float z = res * cos(thetaAngle) / resolution_[2];
                vertices_.push_back(Vertex(
                    x, y, z,
                    x / xLen, y / yLen, z / zLen
                ));
            }
        }
        unsigned int numRows = resolution_[0];
        unsigned int numCols = resolution_[1];
        for (unsigned int i = 0; i < numRows; i++)
        {
            for(unsigned int j = 0; j < numCols; j++)
            {
                indices_.push_back(i * (numRows + 1) + j);
                indices_.push_back(i * (numRows + 1) + j + 1);
                indices_.push_back((i + 1)* (numRows + 1)+ j);

                indices_.push_back(i * (numRows + 1) + j + 1);
                indices_.push_back((i + 1) * (numRows + 1) + j + 1);
                indices_.push_back((i + 1) * (numRows + 1) + j);
            }
        }
    }

    float SphericalPlotCalculator::function(float theta, float fi)
    {
        recursiveDescent_.getElementFromArgumentTable("θ") = Double(theta);
        recursiveDescent_.getElementFromArgumentTable("φ") = Double(fi);

        return (double)dynamic_cast<Double&>(*recursiveDescent_.calculate(expression_).getOperand());
    }
}
