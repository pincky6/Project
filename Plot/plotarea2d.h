#ifndef PLOTAREA2D_H
#define PLOTAREA2D_H

#include <QWidget>
#include "iplotbuilder.h"

class PlotArea2D: public QWidget
{
    Q_OBJECT

public:
    PlotArea2D(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void setExpressions(const std::vector<QString>&);
    ~PlotArea2D();

private:
    PlotType getTypeOfPlot(const QString&);

    void paintAXCoord(QPainter& painter);
    void paintAYCoord(QPainter& painter);

    void paintXGrid(QPainter& painter);
    void paintYGrid(QPainter& painter);

    void paintYCoord(QPainter& painter);
    void paintXCoord(QPainter& painter);

    void paintXYCoordAndGrid(QPainter& painter);
private:
    std::vector<QString> expressionsVector;
};

#endif
