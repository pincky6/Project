#ifndef PLOTTABLE2D_H
#define PLOTTABLE2D_H

#include <QVector>

#include <QSharedPointer>

#include "abstracttable.h"

#include "Marching Squares/marchingsquares.h"

struct Plot2D
{
public:
    Plot2D(const QString&, const QByteArray&);
    Plot2D(const QString&, marching_squares::EdgeList*);

    QByteArray serializeEdgeList();
    QByteArray serializeEdgeList() const;
    void deserializeEdgeList(const QByteArray&);
public:
    QString expression;
    QSharedPointer<marching_squares::EdgeList > vertices;
};

class PlotTable2D: public AbstractTable
{
public:
    bool create() override;
    bool removeByExpression(const QString&) override;
    bool insert(const Plot2D&);
    bool update(const Plot2D&);
    bool insertOrUpdate(const Plot2D&);
    Plot2D selectByExpression(const QString&);
    bool updateLast(const Plot2D&);
    bool existExpression(const QString&);
    bool drop(unsigned int);
    int getRowCount();
    int getMaxId();
};

#endif // PLOTTABLE2D_H
