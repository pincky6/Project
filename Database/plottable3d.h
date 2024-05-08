#ifndef PLOTTABLE3D_H
#define PLOTTABLE3D_H

#include <QDataStream>
#include <QString>

#include <vector>
#include <memory.h>

#include "Marching Cubes/types.h"

#include "abstracttable.h"

#include "serializablecontainers.h"


struct Plot3D: public SerializableContainers
{
public:
    Plot3D(const QString&, QByteArray,
           QByteArray, float);
    Plot3D(const QString&, std::shared_ptr<std::vector<Vertex>>,
           std::shared_ptr<std::vector<unsigned int>>, float);

    QByteArray serializeVertices();
    QByteArray serializeIndices();
    QByteArray serializeVertices() const;
    QByteArray serializeIndices() const;

    void deserializeVertices(QByteArray);
    void deserializeIndices(QByteArray);

    ~Plot3D();
public:
    QString expression;
    std::shared_ptr<std::vector<Vertex>> vertices;
    std::shared_ptr<std::vector<unsigned int>> indices;
    float maxScaleFactor;
};

class PlotTable3D: public AbstractTable
{
public:
    bool create() override;
    bool insert(const Plot3D&);
    bool update(const Plot3D&);
    Plot3D selectByExpression(const QString&);
    bool removeByExpression(const Plot3D&);

    bool existExpression(const QString&);
};

#endif // PLOTTABLE3D_H
