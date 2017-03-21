//Class that models a shape which is made of an array of other shapes of the same type

#ifndef COMPOSITE_H_INCLUDE
#define COMPOSITE_H_INCLUDE

#include "object.h"
#include <vector>

class Composite : public Object
{
public:
    
    Composite(int numFigures) : numFigures(numFigures) {
        figures.reserve(numFigures);
    }

    virtual Hit intersect(const Ray &ray);
    virtual Point textureCoordinates(Point p);
    
protected:
    size_t numFigures;
    std::vector<Object *> figures;
};

inline Hit Composite::intersect(const Ray &ray) {
    //Intersect all figures and return the smallest hit
    Hit min = figures[0]->intersect(ray);
    Hit hit;
    
    for (unsigned int i = 1; i < numFigures; i++) {
        hit = figures[i]->intersect(ray);
        if (!isnan(hit.t) && (hit.t < min.t || isnan(min.t))) {
            min = hit;
        }
    }
    
    return min;
}

inline Point Composite::textureCoordinates(Point p) {
    return p;
}

#endif /* end of include guard: COMPOSITE_H_INCLUDE */
