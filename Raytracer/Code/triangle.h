//Class that models a triangle from its 3 vertices v1, v2 and v3.

#ifndef TRIANGLE_H_INCLUDED
#define TRIANGLE_H_INCLUDED

#include "object.h"

class Triangle : public Object
{
public:
    Triangle(Point v1, Point v2, Point v3) : v1(v1), v2(v2), v3(v3) { }

    virtual Hit intersect(const Ray &ray);

    const Point v1;
    const Point v2;
    const Point v3;

private:
    bool isInside(Point v1, Point v2, Point intersect, Vector N);
};
#endif //End of include ward TRIANGLE_H_INCLUDED
