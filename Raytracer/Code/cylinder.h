//Class that models a cylinder (centered in any axis) from the center of the bottom cap p1, the center of the top cap p2 and its radius r.

#ifndef CYLINDER_H_INCLUDE
#define CYLINDER_H_INCLUDE

#include "object.h"

class Cylinder : public Object
{
public:
    Cylinder(Point p1, Point p2, double r) : p1(p1), p2(p2), r(r) { }

    virtual Hit intersect(const Ray &ray);
    virtual Point textureCoordinates(Point p);
    
    const Point p1;
    const Point p2;
    const double r;
private:
    double capHit(Point c, Vector N, Ray ray, double r);
    bool isInsideBody(Point p1, Point p2, Point intersect);
    bool isInsideCap(Point c, Point intersect, double r);

};

#endif /* end of include guard: CYLINDER_H_INCLUDE */
