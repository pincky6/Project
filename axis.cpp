#include "axis.h"

#include <algorithm>
#include <QMatrix4x4>


Axis::Axis(Vertex startPoint, float radius,
           float heightCylinder, float heightCone,
           std::size_t precision)
{
    Vertex upperPoint = startPoint, conePoint = startPoint;
    upperPoint.position.setZ(upperPoint.position.z() + heightCylinder);
    conePoint.position.setZ(conePoint.position.z() + heightCylinder + heightCone);
    auto  cylinder = generateCylinder(startPoint, radius, heightCylinder, precision);
    auto cone = generateCone(upperPoint, conePoint,
                 precision, radius * 2.0f);

    axisPoints = cylinder.first;
    axisIndices = cylinder.second;

    std::size_t sizeIndices = axisPoints.size();
    std::copy(cone.first.begin(), cone.first.end(), std::back_inserter(axisPoints));
    for(std::size_t i = 0; i < cone.second.size(); i++)
    {
        axisIndices.push_back(sizeIndices + cone.second[i]);
    }
}

Axis::VectorAndIndices Axis::generateCylinder(Vertex startPosition, float radius,
                            float height, std::size_t precision)
{
    std::vector<Vertex> cylinderData;
    std::vector<unsigned int> cylinderIndices;
    startPosition.color = QVector3D(0.0f, 1.0f, 0.0f);
    for(std::size_t i = 0; i <= precision; i++)
    {
        float radians = toRadians(i * 360.0f/ precision);
        float x = startPosition.position.x() + cos(radians) * radius;
        float y = startPosition.position.y() + sin(radians) * radius;
        cylinderData.emplace_back(x, y, startPosition.position.z());

        cylinderData[i].normal.setX(x);
        cylinderData[i].normal.setY(y);
        cylinderData[i].normal.setZ(0.0f);

        cylinderData[i].color = QVector3D(0.0f, 1.0f, 0.0f);
    }
    for(std::size_t i = 0; i <= precision; i++)
    {
        cylinderData.emplace_back(cylinderData[i].position.x(),
                                  cylinderData[i].position.y(),
                                  startPosition.position.z() + height);
        cylinderData[i + precision].normal.setX(cylinderData[i].position.x());
        cylinderData[i + precision].normal.setY(cylinderData[i].position.y());
        cylinderData[i + precision].normal.setZ(0.0f);
        cylinderData[i].color = QVector3D(0.0f, 1.0f, 0.0f);
    }
    Vertex lowerPoint = startPosition;
    lowerPoint.normal.setX(lowerPoint.position.x());
    lowerPoint.normal.setY(lowerPoint.position.y());
    lowerPoint.normal.setZ(-1);
    Vertex upperPoint = startPosition;
    upperPoint.position.setZ(upperPoint.position.z() + height);
    upperPoint.normal.setX(upperPoint.position.x());
    upperPoint.normal.setY(upperPoint.position.y());
    upperPoint.normal.setZ(1);
    cylinderData.emplace_back(lowerPoint.position.x(), lowerPoint.position.y(),
                              lowerPoint.position.z());
    cylinderData.emplace_back(upperPoint.position.x(), upperPoint.position.y(),
                              upperPoint.position.z());

    for(std::size_t i = 0; i <= precision; i++)
    {
        cylinderIndices.push_back(i + precision);
        cylinderIndices.push_back(i + 1);
        cylinderIndices.push_back(i + precision + 1);
        cylinderIndices.push_back(i);
        cylinderIndices.push_back(i + 1);
        cylinderIndices.push_back(i + precision);
    }

    for(std::size_t i = 0; i <= precision; i++)
    {
        cylinderIndices.push_back(i);
        cylinderIndices.push_back(cylinderData.size() - 2);
        cylinderIndices.push_back(i + 1);
    }
    for(std::size_t i = 0; i <= precision; i++)
    {
        cylinderIndices.push_back(i + precision);
        cylinderIndices.push_back(cylinderData.size() - 1);
        cylinderIndices.push_back(i + precision + 1);
    }
    return std::make_pair(cylinderData, cylinderIndices);
}

Axis::VectorAndIndices Axis::generateCone(Vertex startPosition, Vertex endPosition,
                  std::size_t precision, float radius)
{
    std::vector<Vertex> coneData;
    std::vector<unsigned int> coneIndices;
    Vertex middlePoint((endPosition.position.x() + startPosition.position.x()) / 2.0f,
                      (endPosition.position.y() + startPosition.position.y()) / 2.0f,
                      (endPosition.position.z() + startPosition.position.z()) / 2.0f);
    float height = middlePoint.position.z() - startPosition.position.z();
    float middleRadius = radius / 2.0f;
    float coneAngle = atan(radius/ height);
    float flank_len = sqrt(radius * radius + height * height);
    float cone_x = radius / flank_len;
    float cone_y = - height/ flank_len;
    float middleConeAngle = atan(radius/ (endPosition.position.z() - middlePoint.position.z()));
    endPosition.normal.setX(endPosition.position.x() /
                            (fabs(endPosition.position.x()) + fabs(startPosition.position.x())));
    endPosition.normal.setY(endPosition.position.y() /
                            (fabs(endPosition.position.y()) + fabs(startPosition.position.y())));
    endPosition.normal.setZ(endPosition.position.z() /
                            (fabs(endPosition.position.z()) + fabs(startPosition.position.z())));
    for(std::size_t i = 0; i <= precision; i++)
    {
        float radians = toRadians(i * 360.0f/ precision);
        float x = startPosition.position.x() + cos(radians) * radius;
        float y = startPosition.position.y() + sin(radians) * radius;
        coneData.emplace_back(x, y, startPosition.position.z());

        coneData[i].normal.setX(-cone_y * cos(cone_y));
        coneData[i].normal.setY(cone_x);
        coneData[i].normal.setZ(-cone_y * sin(cone_y));

        coneData[i].color = QVector3D(0.0f, 1.0f, 0.0f);
    }
    for(std::size_t i = 0; i < precision; i++)
    {
        float radians = toRadians(i * 360.0f/ precision);
        float x = middlePoint.position.x() + cos(radians) * middleRadius;
        float y = middlePoint.position.y() + sin(radians) * middleRadius;
        coneData.emplace_back(x, y, middlePoint.position.z());

        coneData[i + precision].normal.setX(-cone_y * cos(cone_x));
        coneData[i + precision].normal.setY(cone_x);
        coneData[i + precision].normal.setZ(-cone_y * sin(cone_x));
        coneData[i].color = QVector3D(0.0f, 1.0f, 0.0f);
    }
    for(std::size_t i = 0; i <= precision; i++)
    {
        coneIndices.push_back(i + precision);
        coneIndices.push_back(i + 1);
        coneIndices.push_back(i + precision + 1);
        coneIndices.push_back(i);
        coneIndices.push_back(i + 1);
        coneIndices.push_back(i + precision);
    }
    coneData.push_back(startPosition);
    coneData.push_back(endPosition);
    for(std::size_t i = 0; i <= precision; i++)
    {
        coneIndices.push_back(i);
        coneIndices.push_back(coneData.size() - 2);
        coneIndices.push_back(i + 1);
    }
    for(std::size_t i = 0; i <= precision; i++)
    {
        coneIndices.push_back(i + precision);
        coneIndices.push_back(coneData.size() - 1);
        coneIndices.push_back(i + precision + 1);
    }
    return std::make_pair(coneData, coneIndices);
}

float Axis::toRadians(float degrees)
{
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}
