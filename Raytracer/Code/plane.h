//Class that models an infinite plane. It can be made from a point point and the plane normal N or from 3 points of the plane p1, p2 and p3.

#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "object.h"

class Plane : public Object
{
public:
    
    Plane(Point point, Vector N) : point(point), N(N) {}
    
    Plane(Point p1, Point p2, Point p3) : point(p1), N((p2 - p1).cross(p3 - p1)) {}
    
    virtual Hit intersect(const Ray &ray);
    
    const Point point;
    const Vector N;
};

#endif
