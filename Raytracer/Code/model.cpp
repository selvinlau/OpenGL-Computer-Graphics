
#include "model.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "hit.h"
#include "triangle.h"
#include "glm.h"

Model::Model(unsigned int numTriangles, GLMtriangle *tris, float *vertices) {
    genTriangles(numTriangles, tris, vertices);
}

Hit Model::intersect(const Ray &ray) {
    std::vector<Hit> hits(triangles.size());
    
    for (unsigned int i = 0; i < triangles.size(); i++) {
        hits[i] = triangles[i]->intersect(ray);
    }
    return minHit(hits);
}

Hit Model::minHit(vector<Hit> hits) {
    int indx = 0;
    double mint = hits[indx].t;
    
    for (unsigned int i = 1; i < triangles.size(); i++) {
        if (!isnan(hits[i].t) && (hits[i].t < mint || isnan(mint))) {
            mint = hits[i].t;
            indx = i;
        }
    }
    
    return hits[indx];
}

void Model::genTriangles(unsigned int numTriangles, GLMtriangle *tris, float *vertices) {
    unsigned int *vindices;
    std::vector<Point> triPoints(3);
    
    for (unsigned int i = 0; i < numTriangles; i++) {
        vindices = tris[i].vindices;
        for (unsigned int j = 0; j < 3; j++) {
            triPoints[j] = Point(vertices[3 * vindices[j]], vertices[3 * vindices[j] + 1], vertices[3 * vindices[j] + 2]) + Point(140,300,400);
        }
        triangles.push_back(new Triangle(triPoints[0], triPoints[1], triPoints[2]));
    }
}

Point Model::textureCoordinates(Point p) {
    return p;
}
