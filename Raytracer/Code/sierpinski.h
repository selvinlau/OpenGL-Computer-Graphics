//Class that models a cylinder (centered in any axis) from the center of the bottom cap p1, the center of the top cap p2 and its radius r.
#ifndef SIERPINSKI_H_INCLUDE
#define SIERPINSKI_H_INCLUDE

#include "object.h"
#include <vector>
#include "tetrahedron.h"

class Sierpinski : public Object
{
public:
    
    Sierpinski(Point center, double side, int recursions);

    virtual Hit intersect(const Ray &ray);
    virtual Point textureCoordinates(Point p);
    
    Point center;
    double side;
    int recursions;
    int numFigures;
private:
    std::vector<Tetrahedron *> figures;
    void genTetrahedrons(Tetrahedron *tetra, int recursions);
    Hit minHit(vector<Hit> hits);
};

#endif /* end of include guard: SIERPINSKI_H_INCLUDE */
