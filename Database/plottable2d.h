#ifndef PLOTTABLE2D_H
#define PLOTTABLE2D_H

#include <QVector>

#include <QSharedPointer>

#include "abstracttable.h"

#include "Marching Squares/marchingsquares.h"

#include "serializablecontainers.h"

struct Plot2DImplicit
{
public:
    Plot2DImplicit(const QString&, const QByteArray&, int, int);
    Plot2DImplicit(const QString&, marching_squares::EdgeList*, int, int);

    QByteArray serializeEdgeList();
    QByteArray serializeEdgeList() const;
    void deserializeEdgeList(const QByteArray&);
public:
    QString expression;
    QSharedPointer<marching_squares::EdgeList> vertices;
    int width;
    int height;
};

struct Plot2D: public SerializableContainers
{
public:
    Plot2D(const QString&, const QByteArray&, int, int);
    Plot2D(const QString&, QSharedPointer<std::vector<QPointF>>, int, int);

    QByteArray serializeVertices();
    QByteArray serializeVertices() const;

    void deserializeVertices(QByteArray);

    void deserializeEdgeList(const QByteArray&);
public:
    QString expression;
    QSharedPointer<std::vector<QPointF>> vertices;
    int width;
    int height;
};

class PlotTable2D: public AbstractTable
{
public:
    bool create() override;
    bool removeByExpression(const QString&) override;
    bool insert(const Plot2DImplicit&);
    bool update(const Plot2DImplicit&);
    bool insertOrUpdate(const Plot2DImplicit&);
    Plot2DImplicit selectByExpression(const QString&);
    bool updateLast(const Plot2DImplicit&);

    bool insert(const Plot2D&);
    bool update(const Plot2D&);
    bool insertOrUpdate(const Plot2D&);
    Plot2D selectByExpressionPlot(const QString&);
    bool updateLast(const Plot2D&);

    bool existExpression(const QString&);
    bool existExpression(const QString&, int, int);
    bool drop(unsigned int);
    int getRowCount();
    int getMaxId();

private:
    bool insert(const QString&, QByteArray, int width, int height);
    bool update(const QString&, QByteArray, int width, int height);
    bool insertOrUpdate(const QString&, QByteArray, int width, int height);
    bool updateLast(const QString&, QByteArray, int width, int height);
};

#endif // PLOTTABLE2D_H
