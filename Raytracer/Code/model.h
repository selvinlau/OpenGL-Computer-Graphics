//Class that represents a figure comprised of an array of several triangles

#ifndef MODEL_H_INCLUDE
#define MODEL_H_INCLUDE

#include "object.h"
#include <vector>
#include "triangle.h"
#include "glm.h"
#include "composite.h"

class Model : public Composite
{
public:
    Model(unsigned int numTriangles, GLMtriangle *tris, float *vertices, Point position, double scale);
    
private:
    void genTriangles(unsigned int numTriangles, GLMtriangle *tris, float *vertices, Point position, double scale);
};

#endif /* end of include guard: MODEL_H_INCLUDE */
