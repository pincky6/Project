#include "cylindricalplotcalculator.h"
#include <algorithm>
#include <QDebug>
namespace plot_builder
{
    CylindricalPlotCalculator::CylindricalPlotCalculator(const QString& expression,
                                         Range roRange, Range secondRange, Range thirdRange_,
                                         Resolution resolution, Resolution screenResolution,
                                         QObject* parent):
    AbstractPlotCalculator(expression, secondRange, roRange, thirdRange_,
                               resolution, screenResolution, parent)
    {
        recursiveDescent_.addArgumentToTable("φ", firstRange_[0]);
        recursiveDescent_.addArgumentToTable("ρ", secondRange_[0]);
        recursiveDescent_.addArgumentToTable("z", thirdRange_[0]);
    }

    CylindricalPlotCalculator::CylindricalPlotCalculator(CylindricalPlotCalculator&& calculator) noexcept:
        AbstractPlotCalculator(std::move(calculator))
    {
        recursiveDescent_.addArgumentToTable("φ", firstRange_[0]);
        recursiveDescent_.addArgumentToTable("ρ", secondRange_[0]);
        recursiveDescent_.addArgumentToTable("z", thirdRange_[0]);
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
        calulate();
    }

    void CylindricalPlotCalculator::calulate()
    {
        if(firstRange_[0] < 0)
        {
            firstRange_[1] += fabs(firstRange_[0]);
            firstRange_[0] = 0;
        }

        if(firstRange_[0] > firstRange_[1]) std::swap(firstRange_[0], firstRange_[1]);
        if(secondRange_[0] > secondRange_[1]) std::swap(secondRange_[0], secondRange_[1]);
        if(thirdRange_[0] > thirdRange_[1]) std::swap(thirdRange_[0], thirdRange_[1]);


        float dFi = float(firstRange_[1] - firstRange_[0]) / (float)resolution_[0];
        float dz = float(thirdRange_[1] - thirdRange_[0]) / (float)resolution_[2];


        float xLen = fabs(firstRange_[0]) + fabs(firstRange_[1]);
        float yLen = fabs(secondRange_[0]) + fabs(secondRange_[1]);
        float zLen = fabs(thirdRange_[0]) + fabs(thirdRange_[1]);

        float fiLower = firstRange_[0];
        float zLower = thirdRange_[0];

        for(int k = 0; k < resolution_[2]; k++)
        {
            float z = zLower + float(k) * dz;
            for (int i = 0; i <= resolution_[0]; i++)
            {
                float angle = fiLower + float(i)* dFi;

                float res = function(angle, 0.0f, z);

                float x = res * cos(angle) / screenResolution_[0];
                float y = res * sin(angle) / screenResolution_[1];

                vertices_.push_back(Vertex(
                    x,
                    y,
                    z / screenResolution_[2],
                    x / xLen,
                    y / yLen,
                    z / (screenResolution_[2]* zLen)
                    ));
            }
        }
        unsigned int numRows = resolution_[2];
        unsigned int numCols = resolution_[0];
        for (unsigned int i = 0; i < numRows; i++) {
            for(unsigned int j = 0; j < numCols - 1; j++)
            {
//                for (unsigned int j = 0; j <= numCols - 1; j++) {
//                    // Calculate indices for the four vertices of the current cell
//                    unsigned int index0 = i * numCols + j;
//                    unsigned int index1 = index0 + 1;
//                    unsigned int index2 = (i + 1) * numCols + j;
//                    unsigned int index3 = index2 + 1;

//                    // Add indices to form two triangles
//                    indices_.push_back(index0);
//                    indices_.push_back(index1);
//                    indices_.push_back(index2);

//                    indices_.push_back(index1);
//                    indices_.push_back(index3);
//                    indices_.push_back(index2);
//                }
                indices_.push_back(i * numRows + j);
                indices_.push_back((i + 1) * numRows + j);
                indices_.push_back(i * numRows + j + 1);

                indices_.push_back(i * numRows + j + 1);
                indices_.push_back((i + 1) * numRows + j + 1);
                indices_.push_back((i + 1) * numRows + j);
            }
        }
       // qDebug() << vertices_.size() << indices_.size() << *std::max_element(indices_.begin(), indices_.end());
    }

    float CylindricalPlotCalculator::function(float fi, float ro, float z)
    {
        recursiveDescent_.getElementFromArgumentTable("φ") = Double(fi);
        recursiveDescent_.getElementFromArgumentTable("ρ") = Double(ro);
        recursiveDescent_.getElementFromArgumentTable("z") = Double(z);

        return (double)dynamic_cast<Double&>(*recursiveDescent_.calculate(expression_).getOperand());
    }
}
