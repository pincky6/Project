#ifndef AXIS_H
#define AXIS_H

#include "Marching Cubes/types.h"

struct Axis
{
    using VectorAndIndices = std::pair<std::vector<Vertex>,
                                       std::vector<unsigned int> >;
public:
    Axis(Vertex startPoint, float radius,
         float heightCylinder, float heightCone,
         std::size_t precision);
private:
    VectorAndIndices generateCylinder(Vertex startPosition, float radius,
                          float height, std::size_t precision);
    VectorAndIndices generateCone(Vertex startPosition, Vertex endPosition,
                      std::size_t precision, float radius);
    float toRadians(float degrees);
public:
    std::vector<Vertex> axisPoints;
    std::vector<unsigned int> axisIndices;
};

#endif // AXIS_H
