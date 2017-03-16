//
//  Framework for a raytracer
//  File: triple.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef Matrix3_H_INCLUDED
#define Matrix3_H_INCLUDED

#include <math.h>
#include <iostream>
#include "triple.h"
#include <vector>

using namespace std;

class Matrix3
{
public:
    Matrix3() {}
    Matrix3(Triple r1, Triple r2, Triple r3);
    Triple operator*(Triple vector) const;
    
private:
    std::vector<Triple> mData;
};

inline Matrix3::Matrix3(Triple r1, Triple r2, Triple r3) : mData(3){
    mData[0] = r1;
    mData[1] = r2;
    mData[2] = r3;
}

inline Triple Matrix3::operator*(Triple vector) const
{
    Triple result;
    result.x = mData[0].dot(vector);
    result.y = mData[1].dot(vector);
    result.z = mData[2].dot(vector);
    
    return result;
}


#endif /* end of include guard: Matrix3_H_INCLUDED */
