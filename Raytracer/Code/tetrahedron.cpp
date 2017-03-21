
#include "tetrahedron.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "hit.h"
#include "triangle.h"
#include "composite.h"

Tetrahedron::Tetrahedron(Point center, double side) : Composite(FACES), center(center), side(side), vertices(FACES) {
    genTriangles(center, side);
}

void Tetrahedron::genTriangles(Point center, double side) {
    
    Point top = Point(0, 1, 1 / sqrt(2)) * side / 2 + center;
    Point b1 = Point(1, 0, -1 / sqrt(2)) * side / 2 + center;
    Point b2 = Point(-1, 0, -1 / sqrt(2)) * side / 2 + center;
    Point b3 = Point(0, -1, 1 / sqrt(2)) * side / 2 + center;
    
    vertices[0] = top;
    vertices[1] = b1;
    vertices[2] = b2;
    vertices[3] = b3;
    
    figures.push_back(new Triangle(top, b1, b2));
    figures.push_back(new Triangle(top, b2, b3));
    figures.push_back(new Triangle(top, b3, b1));
    figures.push_back(new Triangle(b1, b2, b3));
    
}
