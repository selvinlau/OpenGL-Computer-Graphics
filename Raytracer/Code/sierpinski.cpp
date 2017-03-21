
#include "sierpinski.h"
#include "tetrahedron.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "hit.h"
#include "composite.h"

Sierpinski::Sierpinski(Point center, double side, int recursions) : Composite(pow(Tetrahedron::FACES, recursions)), center(center), side(side), recursions(recursions) {
    genTetrahedrons(new Tetrahedron(center, side), recursions);
}

void Sierpinski::genTetrahedrons(Tetrahedron *tetra, int recursions) {
    
    Point c1 = (tetra->center + tetra->vertices[0]) / 2;
    Point c2 = (tetra->center + tetra->vertices[1]) / 2;
    Point c3 = (tetra->center + tetra->vertices[2]) / 2;
    Point c4 = (tetra->center + tetra->vertices[3]) / 2;
    
    double side = tetra->side / 2;
    if (recursions == 1) {
        figures.push_back(new Tetrahedron(c1, side));
        figures.push_back(new Tetrahedron(c2, side));
        figures.push_back(new Tetrahedron(c3, side));
        figures.push_back(new Tetrahedron(c4, side));
    } else {
        genTetrahedrons(new Tetrahedron(c1, side), recursions - 1);
        genTetrahedrons(new Tetrahedron(c2, side), recursions - 1);
        genTetrahedrons(new Tetrahedron(c3, side), recursions - 1);
        genTetrahedrons(new Tetrahedron(c4, side), recursions - 1);
    }
}
