//Class that models a regular tetrahedron.

#ifndef TETRAHEDRON_H_INCLUDE
#define TETRAHEDRON_H_INCLUDE

#include "composite.h"
#include <vector>
#include "triangle.h"

class Tetrahedron : public Composite
{
public:
    static const int FACES = 4;
    
    Tetrahedron(Point center, double side);
    
    Point center;   //Center of the tetrahedron
    double side;    //Side length of the tetrahedron
    std::vector<Point> vertices;
    
private:
    void genTriangles(Point center, double side);
};

#endif /* end of include guard: TETRAHEDRON_H_INCLUDE */
