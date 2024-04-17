#include "Database/plottable3d.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QString>

Plot3D::Plot3D(const QString& expression_, QByteArray serializedVertices,
               QByteArray serializedIndices, float maxScaleFactor_):
    expression(expression_), vertices(new std::vector<Vertex>()),
    indices(new std::vector<unsigned int>()), maxScaleFactor(maxScaleFactor_)
{
    deserializeVertices(serializedVertices);
    deserializeIndices(serializedIndices);
}

Plot3D::Plot3D(const QString& expression_, std::shared_ptr<std::vector<Vertex>> vertices_,
               std::shared_ptr<std::vector<unsigned int>> indices_, float maxScaleFactor_):
    expression(expression_), vertices(vertices_),
    indices(indices_), maxScaleFactor(maxScaleFactor_)
{}

QByteArray Plot3D::serializeVertices()
{
    return serializeContainer(*vertices);
}

QByteArray Plot3D::serializeIndices()
{
    return serializeContainer(*indices);
}

QByteArray Plot3D::serializeVertices() const
{
    return serializeContainer(*vertices);
}

QByteArray Plot3D::serializeIndices() const
{
    return serializeContainer(*indices);
}

void Plot3D::deserializeVertices(QByteArray serializedVertices)
{
//    deserializeContainer((*vertices), serializedVertices);
    QDataStream streamVert(&serializedVertices, QDataStream::ReadOnly);
    while(!streamVert.atEnd())
    {
        Vertex element;
        streamVert >> element;
        vertices->push_back(element);
    }
}

void Plot3D::deserializeIndices(QByteArray serializedIndices)
{
    QDataStream streamVert(&serializedIndices, QDataStream::ReadOnly);
    while(!streamVert.atEnd())
    {
        unsigned int element;
        streamVert >> element;
        indices->push_back(element);
    }
}

Plot3D::~Plot3D()
{

}

bool PlotTable3D::create()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS plot3D ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " expression TEXT,"
                    " vertices BLOB,"
                    " indices BLOB,"
                    " maxScaleFactor REAL"
                    ");");
    query.exec();
    qDebug() << "CREATE: " << query.lastError().text();
    return query.next();
}

bool PlotTable3D::insert(const Plot3D& plot3D)
{
    QSqlQuery query;
    query.prepare("INSERT INTO plot3D ("
                  " expression,"
                  " vertices,"
                  " indices,"
                  " maxScaleFactor) "
                  "VALUES ("
                  " :expression,"
                  " :vertices,"
                  " :indices,"
                  " :maxScaleFactor"
                  ");");
    query.bindValue(":expression", plot3D.expression);
    query.bindValue(":vertices", plot3D.serializeVertices());
    query.bindValue(":indices", plot3D.serializeIndices());
    query.bindValue(":maxScaleFactor", plot3D.maxScaleFactor);
    qDebug() << "INSERT: " << query.lastError().text();
    return query.exec();
}

bool PlotTable3D::update(const Plot3D& plot3D)
{
    QSqlQuery query;
    query.prepare("UPDATE plot3D SET"
                  " vertices=:vertices, indices=:indices, maxScaleFactor = :maxScaleFactor"
                  " WHERE expression=:expression ;");
    QByteArray verticesArray = plot3D.serializeVertices();
    QByteArray indicesArray = plot3D.serializeIndices();
    query.bindValue(":vertices", verticesArray);
    query.bindValue(":indices", indicesArray);
    query.bindValue(":maxScaleFactor", plot3D.maxScaleFactor);
    return query.exec();
}

Plot3D PlotTable3D::selectByExpression(const QString& selectExpression)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM plot3D WHERE expression = :expression ;");
    query.bindValue(":expression", selectExpression);
    query.exec();
    query.next();
    QString expression = query.value("expression").toString();
    QByteArray arrayVertices = query.value("vertices").toByteArray();
    QByteArray arrayIndices = query.value("indices").toByteArray();
    float maxScaleValue = query.value("maxScaleFactor").toFloat();
    qDebug() << "SELECT BY EXPRESSION: " << query.lastError().text();
    return Plot3D(expression, arrayVertices, arrayIndices, maxScaleValue);
}

bool PlotTable3D::existExpression(const QString& expression)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM plot3D WHERE expression = :expression;");
    query.bindValue(":expression", expression);
    query.exec();
    qDebug() << "EXIST EXPRESSION: " << query.lastError().text();
    return query.next();
}

bool PlotTable3D::removeByExpression(const Plot3D& plot3D)
{
    QSqlQuery query;
    query.prepare("DELETE FROM plot3D WHERE expression = :expression ;");
    query.bindValue(":expression", plot3D.expression);
    query.exec();
    qDebug() << "REMOVE BY EXPRESSION: " << query.lastError().text();
    return query.next();
}

