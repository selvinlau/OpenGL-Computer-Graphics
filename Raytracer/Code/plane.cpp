
#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Plane **********************************/

Hit Plane::intersect(const Ray &ray)
{
    //Compute ray-plane intersection

    double LdotN = ray.D.dot(N);
    if (LdotN <= 0) {
        return Hit::NO_HIT();
    }
    
    double t = ((point - ray.O).dot(N)) / LdotN;
    if (t < 0) {
        return Hit::NO_HIT();
    }
    
    return Hit(t,N);
}
