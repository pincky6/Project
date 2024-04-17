#ifndef PLOTTABLE2D_H
#define PLOTTABLE2D_H

#include <QVector>

#include "abstractplottable.h"

#include "serializablecontainers.h"


struct Plot2D: public SerializableContainers
{
    using EdgeList = std::array<std::array<double, 2>, 2>;
public:
    Plot2D(const QString&, const QByteArray&);
    Plot2D(const QString&, std::vector<EdgeList>*);
private:
    void serializeEdgeList(const std::vector<EdgeList>&);
    void deserializeEdgeList(const QByteArray&);
public:
    QString expression;
    std::shared_ptr<std::vector<EdgeList> > vertices;
};

class PlotTable2D: public AbstractPlotTable
{
public:
    bool create() override;
    void insert(const Plot2D&);
    void update(const Plot2D&);
    void remove(const Plot2D&);
    void selectAll();
    void select(const QString&);

    bool existExpression(const QString&);
};

#endif // PLOTTABLE2D_H
