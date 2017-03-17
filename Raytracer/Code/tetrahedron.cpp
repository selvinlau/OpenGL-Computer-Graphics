
#include "tetrahedron.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "hit.h"
#include "triangle.h"

Tetrahedron::Tetrahedron(Point center, double side) : faces(FACES) {
    genTriangles(center, side);
}

Hit Tetrahedron::intersect(const Ray &ray) {
    std::vector<Hit> hits(FACES);
    
    for (unsigned int i = 0; i < FACES; i++) {
        hits[i] = faces[i]->intersect(ray);
    }
    return minHit(hits);
}

Hit Tetrahedron::minHit(vector<Hit> hits) {
    int indx = 0;
    double mint = hits[indx].t;
    
    for (unsigned int i = 1; i < FACES; i++) {
        if (!isnan(hits[i].t) && (hits[i].t < mint || isnan(mint))) {
            mint = hits[i].t;
            indx = i;
        }
    }
    
    return hits[indx];
}

void Tetrahedron::genTriangles(Point center, double side) {
    
    Point b1 = Point(1, 0, -1 / sqrt(2)) * side / 2 + center;
    Point b2 = Point(-1, 0, -1 / sqrt(2)) * side / 2 + center;
    Point b3 = Point(0, -1, 1 / sqrt(2)) * side / 2 + center;
    Point top = Point(0, 1, 1 / sqrt(2)) * side / 2 + center;
    
    cout << top << " " << b1 << " " << b2 << " " << b3 << endl;
    faces[0] = new Triangle(top, b1, b2);
    faces[1] = new Triangle(top, b2, b3);
    faces[2] = new Triangle(top, b3, b1);
    faces[3] = new Triangle(b1, b2, b3);
}

Point Tetrahedron::textureCoordinates(Point p) {
    return p;
}
