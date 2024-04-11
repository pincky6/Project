#ifndef CUBE_H
#define CUBE_H

#include <vector>

#include "Marching Cubes/types.h"

struct Rectangle
{
public:
        Rectangle(float width, float height, float top, float left): Width(width), Height(height), Top(top), Left(left){}
        void divide(int n, std::vector<Rectangle>& result);
private:
        void divideHelper(float width, float height,
                      float top, float left, int n,
                      std::vector<Rectangle>& result);
public:
        float Width;
        float Height;
        float Top ;
        float Left ;
};

class Cube
{
public:
    Cube();
    Cube(Vertex point, float width_, float length_, float height_);
    std::vector<Cube> divide(std::size_t num);
public:
    Vertex originPoint;
    float width;
    float length;
    float height;
};


#endif // CUBE_H
