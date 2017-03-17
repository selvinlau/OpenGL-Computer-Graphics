//Class that models a cylinder (centered in any axis) from the center of the bottom cap p1, the center of the top cap p2 and its radius r.
#ifndef MODEL_H_INCLUDE
#define MODEL_H_INCLUDE

#include "object.h"
#include <vector>
#include "triangle.h"
#include "glm.h"

class Model : public Object
{
public:
    Model(unsigned int numTriangles, GLMtriangle *tris, float *vertices);

    virtual Hit intersect(const Ray &ray);
    virtual Point textureCoordinates(Point p);
    
    std::vector<Triangle *> triangles;
    
private:
    void genTriangles(unsigned int numTriangles, GLMtriangle *tris, float *vertices);
    Hit minHit(vector<Hit> hits);
};

#endif /* end of include guard: MODEL_H_INCLUDE */
