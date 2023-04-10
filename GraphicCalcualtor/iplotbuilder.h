#ifndef IPLOTBUILDER_H
#define IPLOTBUILDER_H
#include <QPainter>
#include <mutex>

enum class PlotType
{
    Explicit, Polar, Implicit
};

class IPlotBuilder
{
public:
    virtual void draw(QPainter&, const QString&, int, int) = 0;
    static std::unique_ptr<IPlotBuilder> makePlotBuilder(PlotType);
    virtual ~IPlotBuilder(){}
};

class ExplicitPlotBuilder: public IPlotBuilder
{
public:
    void draw(QPainter&, const QString&, int, int) override;
};

class PolarPlotBuilder: public IPlotBuilder
{
public:
    void draw(QPainter&, const QString&, int, int) override;
};

class ImplicitPlotBuilder: public IPlotBuilder
{
    struct Rect
    {
        Rect(float width, float height, float top, float left): Width(width), Height(height), Top(top), Left(left){}
        float Width;
        float Height;
        float Top ;
        float Left ;
    };
    std::mutex mut;
public:
    void divideDisplay(float, float, float, float, float ,std::vector<Rect>& result);
    void drawHelper(QPainter&, const QString&, float, float, float, float, float, float, int, int);
    void draw(QPainter&, const QString&, int, int) override;
};

#endif // IPLOTBUILDER_H
