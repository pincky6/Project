#include "plottable2d.h"
#include <QSqlQuery>
#include <QPointF>
#include <QSqlError>
#include "Database/settingstable.h"

Plot2DImplicit::Plot2DImplicit(const QString& expression_,
               const QByteArray& serializedVertices,
               int width_, int height_):
    expression(expression_), vertices(new marching_squares::EdgeList),
    width(width_), height(height_)
{
    deserializeEdgeList(serializedVertices);
}

Plot2DImplicit::Plot2DImplicit(const QString& expression_,
               marching_squares::EdgeList* vertices_,
               int width_, int height_):
    expression(expression_), vertices(vertices_),
    width(width_), height(height_)
{

}

QByteArray Plot2DImplicit::serializeEdgeList()
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

QByteArray Plot2DImplicit::serializeEdgeList() const
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

void Plot2DImplicit::deserializeEdgeList(const QByteArray& serializedVertices)
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


Plot2D::Plot2D(const QString& expression_,
               const QByteArray& serializedVertices,
               int width_, int height_):
    expression(expression_), vertices(new std::vector<QPointF>),
    width(width_), height(height_)
{
    deserializeVertices(serializedVertices);
}

Plot2D::Plot2D(const QString& expression_,
               QSharedPointer<std::vector<QPointF>> vertices_,
               int width_, int height_):
    expression(expression_), vertices(vertices_),
    width(width_), height(height_)
{

}


QByteArray Plot2D::serializeVertices()
{
    return serializeContainer(*vertices);
}

QByteArray Plot2D::serializeVertices() const
{
    return serializeContainer(*vertices);
}

void Plot2D::deserializeVertices(QByteArray serializedVertices)
{
    deserializeContainer((*vertices), serializedVertices);
}

bool PlotTable2D::create()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS plot2D ("
                    " id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
                    " expression TEXT,"
                    " vertices BLOB,"
                    " width INTEGER,"
                    " height INTEGER,"
                    " recordId INTEGER NOT NULL,"
                    " FOREIGN KEY (recordId) REFERENCES records(id) ON DELETE CASCADE"
                    ");");
    bool check = query.exec();
    qDebug() << "CREATE 2D: " << query.lastError().text();

    QSqlQuery plot2DInsertTrigger("CREATE TRIGGER IF NOT EXISTS plot2D_insert_trigger "
                                  "BEFORE INSERT ON plot2D "
                                  "BEGIN "
                                  "INSERT INTO records (recordTypeId) VALUES (2); "
                                  "END; ");
    plot2DInsertTrigger.exec();
    qDebug() << "TRIGGER: " << plot2DInsertTrigger.lastError().text();
    return check;
}

bool PlotTable2D::insert(const Plot2DImplicit& plot2D)
{
    return insert(plot2D.expression, plot2D.serializeEdgeList(),
                  plot2D.width, plot2D.height);
}

bool PlotTable2D::update(const Plot2DImplicit& plot2D)
{
    return update(plot2D.expression, plot2D.serializeEdgeList(),
           plot2D.width, plot2D.width);
}

bool PlotTable2D::updateLast(const Plot2DImplicit& plot2D)
{
    return updateLast(plot2D.expression, plot2D.serializeEdgeList(),
                      plot2D.width, plot2D.width);
}

bool PlotTable2D::insert(const Plot2D& plot2D)
{
    return insert(plot2D.expression, plot2D.serializeVertices(),
                  plot2D.width, plot2D.height);
}

bool PlotTable2D::update(const Plot2D& plot2D)
{
    return update(plot2D.expression, plot2D.serializeVertices(),
                  plot2D.width, plot2D.height);
}

bool PlotTable2D::insertOrUpdate(const Plot2D & plot2D)
{
    return  insertOrUpdate(plot2D.expression, plot2D.serializeVertices(),
                         plot2D.width, plot2D.height);
}

Plot2D PlotTable2D::selectByExpressionPlot(const QString& selectExpression)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM plot2D WHERE expression = :expression ;");
    query.bindValue(":expression", selectExpression);
    query.exec();
    query.next();
    QString expression = query.value("expression").toString();
    QByteArray arrayVertices = query.value("vertices").toByteArray();
    int width = query.value("width").toInt();
    int height = query.value("height").toInt();
    qDebug() << "SELECT BY EXPRESSION: " << query.lastError().text();
    return Plot2D(expression, arrayVertices, width, height);
}

bool PlotTable2D::updateLast(const Plot2D& plot2D)
{
    return updateLast(plot2D.expression, plot2D.serializeVertices(),
                      plot2D.width, plot2D.height);
}

bool PlotTable2D::insertOrUpdate(const Plot2DImplicit& plot2D)
{
    return insertOrUpdate(plot2D.expression, plot2D.serializeEdgeList(),
                          plot2D.width, plot2D.width);
}

Plot2DImplicit PlotTable2D::selectByExpression(const QString& selectExpression)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM plot2D WHERE expression = :expression ;");
    query.bindValue(":expression", selectExpression);
    query.exec();
    query.next();
    QString expression = query.value("expression").toString();
    QByteArray arrayVertices = query.value("vertices").toByteArray();
    int width = query.value("width").toInt();
    int height = query.value("height").toInt();
    qDebug() << "SELECT BY EXPRESSION: " << query.lastError().text();
    return Plot2DImplicit(expression, arrayVertices, width, height);
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

bool PlotTable2D::existExpression(const QString& expression, int width, int height)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM plot2D WHERE expression = :expression "
                  "AND width = :width "
                  "AND height = :height;");
    query.bindValue(":expression", expression);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.exec();
    qDebug() << "EXIST EXPRESSION: " << query.lastError().text();
    return query.next();
}

bool PlotTable2D::drop(unsigned int rowCount)
{
    return AbstractTable::drop("plot2D", rowCount);
}

int PlotTable2D::getRowCount()
{
    return AbstractTable::getRowCount("plot2D");
}

int PlotTable2D::getMaxId()
{
    return AbstractTable::getMaxId("plot2D");
}

bool PlotTable2D::insert(const QString& expression, QByteArray array, int width, int height)
{
    int index = getIndex();
    QSqlQuery query;
    query.prepare("INSERT INTO plot2D ("
                  " expression,"
                  " vertices,"
                  " width,"
                  " height,"
                  " recordId) "
                  "VALUES ("
                  " :expression,"
                  " :vertices,"
                  " :width,"
                  " :height,"
                  " :recordId"
                  ");");
    if(array.isEmpty()) return false;
    query.bindValue(":expression", expression);
    query.bindValue(":vertices", array);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":recordId", index);
    bool check = query.exec();
    qDebug() << "INSERT PLOT2D: " << query.lastError().text();
    return check;
}

bool PlotTable2D::update(const QString& expression, QByteArray array, int width, int height)
{
    QSqlQuery query;
    query.prepare("UPDATE plot2D SET"
                  " vertices=:vertices,"
                  " width = :width,"
                  " height = :height"
                  " WHERE expression=:expression;");

    query.bindValue(":expression", expression);
    query.bindValue(":vertices", array);
    query.bindValue(":width", width);
    query.bindValue(":height", height);

    return query.exec();
}

bool PlotTable2D::insertOrUpdate(const QString& expression, QByteArray array, int width, int height)
{
    SettingsTable table;
    SettingsModel model = table.select();

    return (existExpression(expression) ? update(expression, array, width, height) :
                (model.model2DRecordsCount <= getRowCount()) ? updateLast(expression, array, width, height) :
                insert(expression, array, width, height));
}

bool PlotTable2D::updateLast(const QString& expression, QByteArray array, int width, int height)
{
    int maxId = getMaxId();
    QSqlQuery query;
    query.prepare("UPDATE plot2D SET"
                  " vertices=:vertices,"
                  " width = :width,"
                  " height = :height"
                  " WHERE expression=:expression AND id = :id;");

    query.bindValue(":expression", expression);
    query.bindValue(":vertices", array);
    query.bindValue(":width", width);
    query.bindValue(":height", height);
    query.bindValue(":id", maxId);

    return query.exec();
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
