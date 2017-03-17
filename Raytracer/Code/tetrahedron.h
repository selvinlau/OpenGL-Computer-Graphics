//Class that models a cylinder (centered in any axis) from the center of the bottom cap p1, the center of the top cap p2 and its radius r.
#ifndef Tetrahedron_H_INCLUDE
#define Tetrahedron_H_INCLUDE

#include "object.h"
#include <vector>
#include "triangle.h"

class Tetrahedron : public Object
{
public:
    static const size_t FACES = 4;
    
    Tetrahedron(Point center, double side);

    virtual Hit intersect(const Ray &ray);
    virtual Point textureCoordinates(Point p);
    
    Point center;
    double side;
    std::vector<Point> vertices;
    
private:
    std::vector<Triangle *> faces;
    void genTriangles(Point center, double side);
    Hit minHit(vector<Hit> hits);
};

#endif /* end of include guard: Tetrahedron_H_INCLUDE */
