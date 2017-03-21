
#include "model.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "hit.h"
#include "triangle.h"
#include "glm.h"
#include "composite.h"

Model::Model(unsigned int numTriangles, GLMtriangle *tris, float *vertices, Point position, double scale) : Composite(numTriangles) {
    genTriangles(numTriangles, tris, vertices, position, scale);
}

void Model::genTriangles(unsigned int numTriangles, GLMtriangle *tris, float *vertices, Point position, double scale) {
    
    unsigned int *vindices;
    std::vector<Point> triPoints(3);
    
    for (unsigned int i = 0; i < numTriangles; i++) {
        vindices = tris[i].vindices;
        for (unsigned int j = 0; j < 3; j++) {
            triPoints[j] = Point(vertices[3 * vindices[j]], vertices[3 * vindices[j] + 1], vertices[3 * vindices[j] + 2]) * scale + position;
        }
        figures.push_back(new Triangle(triPoints[0], triPoints[1], triPoints[2]));
    }
}
