//
//  Framework for a raytracer
//  File: sphere.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"
#include <math.h>
#include "matrix3.h"

class Sphere : public Object
{
public:
    Sphere(Point position, double r) : position(position), r(r), rotation(NULL) { }
    Sphere(Point position, double r, Vector axis, double angle) : position(position), r(r), rotation(computeRotationMatrix3(r, axis, angle)) {}
    
    virtual Hit intersect(const Ray &ray);
    virtual Point textureCoordinates(Point p);

    const Point position;
    const double r;
    const Matrix3 *rotation;

private:
    Matrix3 genRotationMatrix(Vector axis, double angle);
    Matrix3 * computeRotationMatrix3(double r, Vector axis, double angle);
    Point rotate(Point point);
};

#endif /* end of include guard: SPHERE_H_115209AE */
