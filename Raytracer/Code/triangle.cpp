
#include "triangle.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>

Hit Triangle::intersect(const Ray &ray)
{
    //Calculating Normal of Triangle plane
    Vector N = (v2 - v1).cross(v3 - v1).normalized();
    
    //Compute the hit point between the ray and the triangles plane
    double LdotN = ray.D.dot(N);
    
    if (LdotN == 0) {
        return Hit::NO_HIT();
    }
    
    double t = (v1 - ray.O).dot(N) / LdotN;
    
    if (t < 0) {
        return Hit::NO_HIT();
    }
    
    Point intersect = ray.at(t);
    //Check if the intersection point is inside the 3 edges of the triangle
    if (! (isInside(v1, v2, intersect, N) && (isInside(v2, v3, intersect, N)) && (isInside(v3, v1, intersect, N)))) {
        return Hit::NO_HIT();
    }
    return Hit(t, N);
}

//Compute the edge that joints the vertices v1 and v2 of the triangle of normal N and check if the itersect point is "inside" it.
bool Triangle::isInside(Point v1, Point v2, Point intersect, Vector N) {
    Vector edge = v2 - v1;
    Vector interV = intersect - v1;
    Vector C = edge.cross(interV);
    
    return (N.dot(C) >= 0);
}

Point Triangle::textureCoordinates(Point p) {
    return p;
}
