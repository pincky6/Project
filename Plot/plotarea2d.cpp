#include "Calculator/recursivedescent.h"
#include "Marching Squares/marchingsquares.h"
#include "plotarea2d.h"
#include <QPainter>
#include <vector>

PlotArea2D::PlotArea2D(QWidget *parent): QWidget(parent)
{

}

PlotArea2D::~PlotArea2D()
{

}

PlotType PlotArea2D::getTypeOfPlot(const QString & expression)
{

    if(expression.indexOf('x') != -1 && expression.indexOf('y') == -1 && expression.indexOf("φ") == -1)
    {
        return PlotType::Explicit;
    }
    if(expression.indexOf('x') != -1 && expression.indexOf('y') != -1 && expression.indexOf("φ") == -1)
    {
        return PlotType::Implicit;
    }
    if(expression.indexOf('x') == -1 && expression.indexOf('y') == -1 && expression.indexOf("φ") != -1)
    {
        return PlotType::Polar;
    }
    return PlotType::Explicit;
}


void PlotArea2D::paintAXCoord(QPainter &painter)
{
    painter.drawLine(0,height()/2, width(),height()/2);
}

void PlotArea2D::paintAYCoord(QPainter &painter)
{
    painter.drawLine(width()/2 ,0 , width()/2, height());
}

void PlotArea2D::paintXCoord(QPainter &painter)
{
    short coordX = 1;
    int displayWidth = width(), displayHeigth = height();
    for(int i = 40; i < displayWidth/2; i += 40)
    {
        painter.drawLine(displayWidth/2 + i, displayHeigth/2 - 5, displayWidth/2 + i, displayHeigth/2 + 5);
        painter.drawLine(displayWidth/2 - i, displayHeigth/2 - 5, displayWidth/2 - i, displayHeigth/2 + 5);

        painter.drawText(displayWidth/2 + i, displayHeigth/2 + 20, QString::number(coordX));
        painter.drawText(displayWidth/2 - i, displayHeigth/2 + 20,  QString::number(-coordX));
        coordX++;
    }
}

void PlotArea2D::paintYCoord(QPainter &painter)
{
    short coordY = 1;
    int displayWidth = width(), displayHeigth = height();
    for(int i = 40; i < displayHeigth/2; i +=40)
    {
        painter.drawLine(displayWidth/2 - 5, displayHeigth/2 + i, displayWidth/2 + 5, displayHeigth/2 + i);
        painter.drawLine(displayWidth/2 + 5, displayHeigth/2 - i, displayWidth/2 - 5, displayHeigth/2 - i);

        painter.drawText(displayWidth/2 + 10, displayHeigth/2 + i,  QString::number(-coordY));
        painter.drawText(displayWidth/2 + 10, displayHeigth/2 - i, QString::number(coordY));
        coordY++;
    }
}

void PlotArea2D::paintXGrid(QPainter &painter)
{
    int displayWidth = width(), displayHeigth = height();
    for(int i = 40; i < displayHeigth/2; i +=40)
    {
        painter.drawLine(0, displayHeigth/2 + i, displayWidth, displayHeigth/2 + i);
        painter.drawLine(0, displayHeigth/2 - i, displayWidth, displayHeigth/2 - i);
    }
}

void PlotArea2D::paintYGrid(QPainter &painter)
{
    int displayWidth = width(), displayHeigth = height();
    for(int i = 40; i < width()/2; i += 40)
    {
        painter.drawLine(displayWidth/2 + i, 0, displayWidth/2 + i, displayHeigth);
        painter.drawLine(displayWidth/2 - i, 0, displayWidth/2 - i, displayHeigth);
    }
}

void PlotArea2D::paintXYCoordAndGrid(QPainter &painter)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(70, 100, 220), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    paintXGrid(painter);
    paintYGrid(painter);

    painter.setPen(QPen(QColor(0, 0, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    paintAXCoord(painter);
    paintAYCoord(painter);
    paintXCoord(painter);
    paintYCoord(painter);
    painter.end();
}

void PlotArea2D::setExpressions(const std::vector<QString>& expressionsVector_)
{
    expressionsVector = expressionsVector_;
}


void PlotArea2D::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter;
    if(expressionsVector.empty())
        return;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0, 0, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    for(std::size_t i = 0; i < expressionsVector.size(); i++)
    {
        auto plotBuilder = IPlotBuilder::makePlotBuilder(getTypeOfPlot(expressionsVector[i]));
        plotBuilder->draw(painter, expressionsVector[i], width(), height());
    }
    painter.end();
    paintXYCoordAndGrid(painter);
}
