#include "plottable2d.h"
#include <QThread>
#include <QSqlQuery>
#include <QSqlError>

Plot2D::Plot2D(const QString& expression_,
               const QByteArray& serializedVertices):
    expression(expression_), vertices(new marching_squares::EdgeList)
{
    deserializeEdgeList(serializedVertices);
}

Plot2D::Plot2D(const QString& expression_,
               marching_squares::EdgeList* vertices_):
    expression(expression_), vertices(vertices_)
{

}

QByteArray Plot2D::serializeEdgeList()
{
    QByteArray buffer;
    QDataStream stream(&buffer, QDataStream::WriteOnly);
    return buffer;
    for(auto&& edges: *vertices)
    {
        for(auto&& edge: edges)
        {
            for(auto&& point: edge)
            {
                stream << point;
            }
        }
    }
    return buffer;
}

QByteArray Plot2D::serializeEdgeList() const
{
    QByteArray buffer;
    QDataStream stream(&buffer, QDataStream::WriteOnly);
    for(auto&& edges: *vertices)
    {
        for(auto&& edge: edges)
        {
            for(auto&& point: edge)
            {
                stream << point;
            }
        }
    }
    return buffer;
}

void Plot2D::deserializeEdgeList(const QByteArray& serializedVertices)
{
    QByteArray buffer = serializedVertices;
    QDataStream stream(&buffer, QDataStream::ReadOnly);
    while(!stream.atEnd())
    {
        std::array<std::array<double, 2>, 2> edges;
        stream >> edges[0][0];
        stream >> edges[0][1];

        stream >> edges[1][0];
        stream >> edges[1][1];
        vertices->push_back(std::move(edges));
    }
   //deserializeContainer(*vertices, serializedVertices);
}

bool PlotTable2D::create()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS plot2D ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " expression TEXT,"
                    " vertices BLOB,"
                    " recordId INTEGER NOT NULL,"
                    " FOREIGN KEY (recordId) REFERENCES records(id) ON DELETE CASCADE"
                    ");");
    bool check = query.exec();
    qDebug() << "CREATE 3D: " << query.lastError().text();

    QSqlQuery plot2DInsertTrigger("CREATE TRIGGER IF NOT EXISTS plot2D_insert_trigger "
                                  "BEFORE INSERT ON plot2D "
                                  "BEGIN "
                                  "INSERT INTO records (recordTypeId) VALUES (2); "
                                  "END; ");
    plot2DInsertTrigger.exec();
    qDebug() << "TRIGGER: " << plot2DInsertTrigger.lastError().text();
    return check;
}

bool PlotTable2D::insert(const Plot2D& plot2D)
{
    int index = getIndex();
    QSqlQuery query;
    query.prepare("INSERT INTO plot2D ("
                  " expression,"
                  " vertices,"
                  " recordId) "
                  "VALUES ("
                  " :expression,"
                  " :vertices,"
                  " :recordId"
                  ");");
    QByteArray array = plot2D.serializeEdgeList();
    if(array.isEmpty()) return false;
    query.bindValue(":expression", plot2D.expression);
    query.bindValue(":vertices", array);
    query.bindValue(":recordId", index);
    bool check = query.exec();
    qDebug() << "INSERT PLOT2D: " << query.lastError().text();
    return check;
}

bool PlotTable2D::update(const Plot2D& plot2D)
{
    QSqlQuery query;
    query.prepare("UPDATE plot2D SET"
                  " vertices=:vertices"
                  " WHERE expression=:expression;");

    QByteArray verticesArray = plot2D.serializeEdgeList();

    query.bindValue(":expression", plot2D.expression);
    query.bindValue(":vertices", verticesArray);

    return query.exec();
}

Plot2D PlotTable2D::selectByExpression(const QString& selectExpression)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM plot2D WHERE expression = :expression ;");
    query.bindValue(":expression", selectExpression);
    query.exec();
    query.next();
    QString expression = query.value("expression").toString();
    QByteArray arrayVertices = query.value("vertices").toByteArray();
    qDebug() << "SELECT BY EXPRESSION: " << query.lastError().text();
    return Plot2D(expression, arrayVertices);
}

bool PlotTable2D::existExpression(const QString& expression)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM plot2D WHERE expression = :expression;");
    query.bindValue(":expression", expression);
    query.exec();
    qDebug() << "EXIST EXPRESSION: " << query.lastError().text();
    return query.next();
}

bool PlotTable2D::removeByExpression(const QString& expression)
{
    QSqlQuery query;
    query.prepare("DELETE FROM plot2D WHERE expression = :expression ;");
    query.bindValue(":expression", expression);
    query.exec();
    qDebug() << "REMOVE BY EXPRESSION: " << query.lastError().text();
    return query.next();
}
