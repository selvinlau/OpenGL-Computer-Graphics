
#include "sierpinski.h"
#include "tetrahedron.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "hit.h"

Sierpinski::Sierpinski(Point center, double side, int recursions) : center(center), side(side), recursions(recursions) {
    numFigures = pow(Tetrahedron::FACES, recursions);
    genTetrahedrons(new Tetrahedron(center, side), recursions);
}

Hit Sierpinski::intersect(const Ray &ray) {
    std::vector<Hit> hits(numFigures);
    
    for (unsigned int i = 0; i < numFigures; i++) {
        hits[i] = figures[i]->intersect(ray);
    }
    //cout << hits[0].t << " " << hits[1].t << " " << hits[2].t <<" " << hits[3].t << endl;
    return minHit(hits);
}

Hit Sierpinski::minHit(vector<Hit> hits) {
    int indx = 0;
    double mint = hits[indx].t;
    
    for (unsigned int i = 1; i < numFigures; i++) {
        if (!isnan(hits[i].t) && (hits[i].t < mint || isnan(mint))) {
            mint = hits[i].t;
            indx = i;
        }
    }
    
    return hits[indx];
}

void Sierpinski::genTetrahedrons(Tetrahedron *tetra, int recursions) {
    
    Point c1 = (tetra->center + tetra->vertices[0]) / 2;
    Point c2 = (tetra->center + tetra->vertices[1]) / 2;
    Point c3 = (tetra->center + tetra->vertices[2]) / 2;
    Point c4 = (tetra->center + tetra->vertices[3]) / 2;
    cout << tetra->vertices[0] << " " << tetra->vertices[1] << " " << tetra->vertices[2] << " " << tetra->vertices[3] << endl;
    cout << c1 << " " << c2 << " " << c3 <<" " << c4 << endl;
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

Point Sierpinski::textureCoordinates(Point p) {
    return p;
}
