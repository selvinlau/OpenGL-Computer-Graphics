//Class that models a Sierpinski tetrahedron.

#ifndef SIERPINSKI_H_INCLUDE
#define SIERPINSKI_H_INCLUDE

#include "object.h"
#include <vector>
#include "tetrahedron.h"
#include "composite.h"

class Sierpinski : public Composite
{
public:
    
    Sierpinski(Point center, double side, int recursions);
    
    Point center;   //Center of the tetrahedron
    double side;    //Side length of the tetrahedron
    int recursions; //Number of splits of the tetrahedron
private:
    void genTetrahedrons(Tetrahedron *tetra, int recursions);
};

#endif /* end of include guard: SIERPINSKI_H_INCLUDE */
