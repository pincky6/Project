/*
    Tables and conventions from
    http://paulbourke.net/geometry/polygonise/
*/

#include <algorithm>
#include <cmath>

#include "marching_cubes.h"

namespace plot_algorithms
{
MarchingCubes::MarchingCubes(): vertices_cnt_(0)
{}

MarchingCubes::MarchingCubes(const QString& expression, RecursiveDecsent& recursiveDescent)
{
    expression_ = expression;
    recursiveDescent_ = recursiveDescent;
}
    int MarchingCubes::calculate_cube_index(GridCell& cell, float isovalue)
    {
        int cubeIndex = 0;
        for (int i = 0; i < 8; i++)
            if (cell.value[i] < isovalue) cubeIndex |= (1 << i);
        return cubeIndex;
    }

    std::vector<Vertex> MarchingCubes::get_intersection_coordinates(GridCell& cell, float isovalue)
    {
        std::vector<Vertex> intersections(12);

        int cubeIndex = calculate_cube_index(cell, isovalue);
        int intersectionsKey = edgeTable[cubeIndex];

        int idx = 0;
        while (intersectionsKey)
        {
            if (intersectionsKey & 1)
            {
                int v1 = edgeToVertices[idx].first, v2 = edgeToVertices[idx].second;
                Vertex intersectionPoint = interpolate(cell.vertex[v1], cell.value[v1],
                                                      cell.vertex[v2], cell.value[v2], isovalue);
                intersections[idx] = intersectionPoint;
            }
            idx++;
            intersectionsKey >>= 1;
        }


        return intersections;
    }

    Vertex MarchingCubes::interpolate(Vertex& v1, float val1, Vertex& v2, float val2, float isovalue)
    {
        Vertex interpolated;
        float mu = (isovalue - val1) / (val2 - val1);

        interpolated.position.setX(mu * ((v2.position.x()) - v1.position.x()) + v1.position.x());
        interpolated.position.setY(mu * ((v2.position.y()) - v1.position.y()) + v1.position.y());
        interpolated.position.setZ(mu * ((v2.position.z()) - v1.position.z()) + v1.position.z());

        return interpolated;
    }

    void MarchingCubes::calculate_triangles(std::vector<Vertex>& intersections,
                                                    int cubeIndex, std::vector<Vertex>& points,
                                                    std::vector<unsigned int>& indices,
                                                    const Range& xRange, const Range& yRange, const Range& zRange)
    {
        float xLen = fabs(xRange[0]) + fabs(xRange[1]);
        float yLen = fabs(yRange[0]) + fabs(yRange[1]);
        float zLen = fabs(zRange[0]) + fabs(zRange[1]);
        for (int i = 0; triangleTable[cubeIndex][i] != -1; i += 3)
        {
            for (int j = 0; j < 3; j++)
            {
                Vertex p = intersections[triangleTable[cubeIndex][i + j]];
                points.emplace_back(p.position.x(), p.position.y(), p.position.z(),
                                    p.position.x() / xLen, p.position.y() / yLen, p.position.z() / zLen);
                indices.push_back(vertices_cnt_ + j);
            }
            vertices_cnt_ += 3;
        }
    }


    void MarchingCubes::triangulate_cell(GridCell& cell, float isovalue, std::vector<Vertex>& points,
                                         std::vector<unsigned int>& indices,
                                         const Range& xRange, const Range& yRange, const Range& zRange)
    {
        int cubeIndex = calculate_cube_index(cell, isovalue);
        std::vector<Vertex> intersections = get_intersection_coordinates(cell, isovalue);
        calculate_triangles(intersections, cubeIndex, points, indices, xRange, yRange, zRange);
    }

    VerticesAndIndices MarchingCubes::triangulate_field(Range xRange, Range yRange, Range zRange,
                                                        Resolution resolution, float isovalue)
    {
        if(xRange[0] > xRange[1]) std::swap(xRange[0], xRange[1]);
        if(yRange[0] > yRange[1]) std::swap(yRange[0], yRange[1]);
        if(zRange[0] > zRange[1]) std::swap(zRange[0], zRange[1]);

        std::vector<Vertex> points;
        std::vector<unsigned int> indices;
        float dx = float(xRange[1] - xRange[0]) / (float)resolution[0];
        float dy = float(yRange[1] - yRange[0]) / (float)resolution[1];
        float dz = float(zRange[1] - zRange[0]) / (float)resolution[2];

        float xLower = xRange[0];
        float yLower = yRange[0];
        float zLower = zRange[0];

        for (int i = 0; i + 1 < resolution[0]; i++)
        {
            const float x = xLower + i * dx;
            for (int j = 0; j + 1 < resolution[1]; j++)
            {
                const float y = yLower + j * dy;
                for (int k = 0; k + 1 < resolution[2]; k++)
                {
                    const float z = zLower + k * dz;

                    GridCell cell =
                        {
                            {
                                {x, y, z},
                                {xLower + (i + 1.0f) * dx, y, z},
                                {xLower + (i + 1.0f) * dx, y, zLower + (k + 1.0f) * dz},
                                {x, y, zLower + (k + 1.0f) * dz},
                                {x, yLower + (j + 1.0f) * dy, z},
                                {xLower + (i + 1.0f) * dx, yLower + (j + 1.0f) * dy, z},
                                {xLower + (i + 1.0f) * dx, yLower + (j + 1.0f) * dy, zLower + (k + 1.0f) * dz},
                                {x, yLower + (j + 1.0f) * dy, zLower + (k + 1.0f) * dz}
                            },
                            {
                                 function(x, y, z),
                                 function(xLower + (i + 1.0f) * dx, y, z),
                                 function(xLower + (i + 1.0f) * dx, y, zLower + (k + 1.0f) * dz),
                                 function(x, y, zLower + (k + 1.0f) * dz),
                                 function(x, yLower + (j + 1.0f) * dy, z),
                                 function(xLower + (i + 1.0f) * dx, yLower + (j + 1.0f) * dy, z),
                                 function(xLower + (i + 1.0f) * dx, yLower + (j + 1.0f) * dy, zLower + (k + 1.0f) * dz),
                                 function(x, yLower + (j + 1.0f) * dy, zLower + (k + 1.0f) * dz)
                            }
                        };
                    triangulate_cell(cell, isovalue, points, indices,
                                    xRange, yRange, zRange);
                }
            }
        }
        vertices_cnt_ = 0;
        return std::make_tuple(points, indices);
    }

    float MarchingCubes::function(float x, float y, float z)
    {
        recursiveDescent_.getElementFromArgumentTable("x") = Double(x);
        recursiveDescent_.getElementFromArgumentTable("y") = Double(y);
        recursiveDescent_.getElementFromArgumentTable("z") = Double(z);

        return (double)dynamic_cast<Double&>(*recursiveDescent_.calculate(expression_).getOperand());
    }
}
