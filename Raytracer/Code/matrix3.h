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
    static const size_t ROWS = 3;
    Matrix3() {}
    Matrix3(Triple r1, Triple r2, Triple r3);
    Matrix3(const Matrix3 &m);
    
    Triple operator*(const Triple &vector) const;
    Matrix3 operator*(double k) const;
    Matrix3 operator+(const Matrix3 &mat) const;
    Triple operator()(int row) const;
    
    friend ostream& operator<<(ostream &s, const Matrix3 &mat);
    
private:
    std::vector<Triple> mData;
};

#endif /* end of include guard: Matrix3_H_INCLUDED */
