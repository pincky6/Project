#ifndef PLOTAREA_H
#define PLOTAREA_H

#include <QWidget>
#include "iplotbuilder.h"

class PlotArea: public QWidget
{
    Q_OBJECT

public:
    PlotArea(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*);
    void setExpressions(const std::vector<QString>&);
    ~PlotArea();

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

#endif // PLOTAREA_H
