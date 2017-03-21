//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <math.h>
#include "matrix3.h"

/************************** Sphere **********************************/

Matrix3 * Sphere::computeRotationMatrix3(double r, Vector axis, double angle) {
    
    if (angle == 0) {
        return NULL;
    }
    
    if (axis.length() != 0) {
        axis = axis.normalized();
    }
    
    //Rotation to map the arbitrary axis to the z-axis
    double zangle;
    Vector zaxis(0.0, 0.0, -1.0);
    zangle = acos(-r * axis.dot(zaxis) * M_PI / 180.0);
    axis = zaxis.cross(axis).normalized();
    
    Matrix3 mapZaxis = genRotationMatrix(axis, zangle);
    
    //Rotation around the z-axis
    Matrix3 rotation = genRotationMatrix(zaxis, angle * M_PI / 180.0);
    
    return new Matrix3(rotation * mapZaxis);
}

Matrix3 Sphere::genRotationMatrix(Vector axis, double angle) {
    Matrix3 identity(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1));
    Matrix3 last(Vector(0, -axis.z, axis.y), Vector(axis.z, 0, -axis.x), Vector(-axis.y, axis.x, 0));
    
    Vector mr1(pow(axis.x, 2), axis.x * axis.y, axis.x * axis.z);
    Vector mr2(axis.y * axis.x, pow(axis.y, 2), axis.y * axis.z);
    Vector mr3(axis.z * axis.x, axis.z * axis.y, pow(axis.z, 2));
    Matrix3 middle(mr1, mr2, mr3);
    
    return Matrix3(identity * cos(angle) + middle * (1 - cos(angle)) + last * sin(angle));
}

Point Sphere::textureCoordinates(Point point) {
    double u;
    double v;
    double theta;
    double phi;
    
    Point p = rotate(point);
    theta = acos(-(p.z - this->position.z) / this->r);
    phi = atan2(p.y - this->position.y, p.x - this->position.x);
    
    if (phi < 0) {
        phi += 2 * M_PI;
    }
    
    u = phi / (2 * M_PI);
    v = (M_PI - theta) / M_PI;
    
    return Point(u, v, 0);
}

Point Sphere::rotate(Point point) {
    if (rotation == NULL) {
        return point;
    }
    
    point -= position;
    Point rotated = (*rotation) * point;
    return rotated + position;
}

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    // place holder for actual intersection calculation

    double b = 2 * (ray.O - position).dot(ray.D);
    double c = (ray.O - position).dot(ray.O - position) - r*r;
    
    double d = b*b - 4*c;
    if (d < 0) {
        return Hit::NO_HIT();
    }
    
    //Take the smaller t that is greater than 0 (it there is any)
    double t = (-b - sqrt(d))/2;
    double t2 = (-b + sqrt(d))/2;
    
    if (t2 < t && t2 >= 0) {
        t = t2;
    }
    
    if (t < 0) {
        return Hit::NO_HIT();
    }
    
    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    Point intersection = ray.at(t);
    Vector N = (intersection - position).normalized();
    
    return Hit(t,N);

}
