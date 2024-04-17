#include "plottable2d.h"

#include <QSqlQuery>

Plot2D::Plot2D(const QString& expression_,
               const QByteArray& serializedVertices):
    expression(expression_), vertices(new std::vector<EdgeList>)
{
    deserializeEdgeList(serializedVertices);
}

Plot2D::Plot2D(const QString& expression,
               std::vector<EdgeList>* vertices)
{

}

void Plot2D::serializeEdgeList(const std::vector<EdgeList>& vertices)
{

}

void Plot2D::deserializeEdgeList(const QByteArray& serializedVertices)
{
   // deserializeContainer(*vertices, serializedVertices);
}

bool PlotTable2D::create()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS plot2D ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " expression TEXT,"
                    " vertices BLOB,"
                    " indices BLOB"
                    ");");
    query.exec();
    return query.next();
}

void PlotTable2D::insert(const Plot2D&)
{

}

void PlotTable2D::update(const Plot2D&)
{

}

void PlotTable2D::selectAll()
{

}

void PlotTable2D::select(const QString &)
{

}

bool PlotTable2D::existExpression(const QString &)
{

}

void PlotTable2D::remove(const Plot2D &)
{

}
