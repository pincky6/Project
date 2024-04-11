#include "figures.h"

Cube::Cube()
    : originPoint(0.0f, 0.0f, 0.0f), width(1.0f), length(1.0f), height(1.0f)
{

}

Cube::Cube(Vertex originPoint_, float width_, float length_, float height_)
    : originPoint(originPoint_), width(width_), length(length_), height(height_)
{
}

std::vector<Cube> Cube::divide(std::size_t num)
{
    std::vector<Cube> dividedCubes;
    std::vector<Rectangle> dividedRectangles;
    Rectangle rect(width, length, originPoint.position.x(), originPoint.position.y());
    rect.divide(num, dividedRectangles);
    for(auto&& rect: dividedRectangles){
        dividedCubes.emplace_back(Vertex(rect.Left, rect.Top, originPoint.position.z()),
                                  rect.Width, rect.Height, height);
    }
    return dividedCubes;
}


void Rectangle::divide(int n, std::vector<Rectangle> &result)
{
    divideHelper(Width, Height, Top, Left, n, result);
}

void Rectangle::divideHelper(float width, float height, float top, float left, int n, std::vector<Rectangle>& result)

{
    if(n == 0 || n == 1)
    {
        result.push_back(Rectangle(width, height, top, left));
        return;
    }
    double l = n / 2.0;
    double r = n - l;

    double newWidth = width;
    double newHeight = height;

    if(width > height)
    {
        newWidth /= 2.0;
        divideHelper(newWidth, newHeight, top, left, l, result);
        divideHelper(newWidth, newHeight, top, left + newWidth, r, result);
    }
    else
    {
        newHeight /= 2.0;

        divideHelper(newWidth, newHeight, top, left, l, result);
        divideHelper(newWidth, newHeight, top + newHeight, left, r, result);
    }
}
