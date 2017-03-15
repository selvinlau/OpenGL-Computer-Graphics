
#include "cylinder.h"
#include <iostream>
#include <math.h>

/************************** Cylinder **********************************/

Hit Cylinder::intersect(const Ray &ray)
{
    Vector H = p2 - p1;
    
    //Compute coefficients of the ray-cylinder body intersection
    Vector AO     = (ray.O - p1);
    Vector AOxH   = AO.cross(H);
    Vector DxH    = ray.D.cross(H);
    double  H2    = H.dot(H);
    
    double a      = DxH.dot(DxH);
    double b      = 2 * DxH.dot(AOxH);
    double c      = AOxH.dot(AOxH) - (r*r * H2);

    double d = b*b - 4*a*c;
    double t = -1;
    double t2;
    
    Point intersect;
    
    if (d >= 0) {
        //Take the smaller t that is greater than 0 (it there is any)
        t = (-b - sqrt(d))/(2*a);
        t2 = (-b + sqrt(d))/(2*a);
        
        if (t2 < t && t2 >= 0) {
            t = t2;
        }
        
        //Check if there is an intersection with the body of the cylinder
        if (t >= 0) {
            intersect = ray.at(t);
            //If the point is outside of the limits of the body, there is no hit
            if (!isInsideBody(p1, p2, intersect) ) {
                t = -1;
                //return Hit::NO_HIT();
            }
        }
    }
    
    //Compute cylinder body normal
    Vector V = intersect - p1;
    Vector N = V - (V.dot(H) * H) / H.dot(H);
    
    //Check if there is a hit with the cylinder bottom cap and compute its normal
    Vector Ncap = (p2 - p1);
    double tcap = capHit(p1, Ncap, ray, r);
    
    //If there is a hit and is closer than the body one, take it
    if ((tcap >= 0 && tcap < t) || t < 0) {
        t = tcap;
        N = Ncap;
    }
    
    //Check if there is a hit with the cylinder top cap and compute its normal
    Ncap = (p1 - p2);
    tcap = capHit(p2, Ncap, ray, r);
    
    //If there is a hit and is closer than the previous ones, take it
    if ((tcap >= 0 && tcap < t) || t < 0) {
        t = tcap;
        N = Ncap;
    }
    
    //If all ts are negative, there is no hit
    if (t < 0) {
        return Hit::NO_HIT();
    }
    
    return Hit(t,N.normalized());
}

//Compute the hit of the ray ray with the cap of center c, normal N and radius r. If there is no hit, return -1
double Cylinder::capHit(Point c, Vector N, Ray ray, double r) {
    double t = -1;
    //Compute the intersection with the plane of the cap
    double LdotN = ray.D.dot(N);
    if (LdotN > 0) {
        t = ((c - ray.O).dot(N)) / LdotN;
        //Check if the point is inside the circle
        if (!isInsideCap(c, ray.at(t), r)) {
            t = -1;
        }
    }
    return t;
}

//Check if a point intersect is inside the limits of the cylinder body with bottom cap center p1 and top cap center p2
bool Cylinder::isInsideBody(Point p1, Point p2, Point intersect) {
    Vector H = p2 - p1;
    return H.dot(intersect - p1) > 0 && H.dot(intersect - p2) < 0;
}

//Check if a point intersect is inside the limits of the circle with center c and radius r
bool Cylinder::isInsideCap(Point c, Point intersect, double r) {
    return (intersect - c).dot(intersect - c) < r*r;
}

Point Cylinder::textureCoordinates(Point p) {
    return p;
}
