#ifndef TYPES_H
#define TYPES_H

#include <map>
#include <vector>
#include <QVector3D>

struct Vertex
{
    QVector3D position;
    QVector3D normal;
    QVector3D color;
    Vertex(): position(0.0f, 0.0f, 0.0f),
              normal(0.0f, 0.0f, 0.0f),
              color(0.0f, 1.0f, 0.0f)
    {}
    Vertex(float x_, float y_, float z_)
        : position(x_, y_, z_)
    {}

    Vertex(float x_, float y_, float z_,
          float norm_x_, float norm_y_,
          float norm_z_)
        : position(x_, y_, z_),
          normal(norm_x_, norm_y_,norm_z_){}
    Vertex(const Vertex&) = default;
    Vertex(Vertex&&) = default;

    Vertex& operator=(const Vertex&) = default;
    Vertex& operator=(Vertex&&) = default;

    friend QDataStream& operator<<(QDataStream& out, const Vertex& vertex)
    {
        out << vertex.position;
        out << vertex.normal;
        out << vertex.color;
        return out;
    }
    friend QDataStream& operator>>(QDataStream& in, Vertex& vertex)
    {
        in >> vertex.position;
        in >> vertex.normal;
        in >> vertex.color;

        return in;
    }
};


struct GridCell
{
    Vertex vertex[8];
    float value[8];
};

#endif
