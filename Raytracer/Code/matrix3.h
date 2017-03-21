//
// Class that models a 3x3 matrix
//

#ifndef MATRIX3_H_INCLUDED
#define MATRIX3_H_INCLUDED

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
    Matrix3 operator*(const Matrix3 &mat) const;
    Matrix3 operator+(const Matrix3 &mat) const;
    Triple operator()(int row) const;
    
    friend ostream& operator<<(ostream &s, const Matrix3 &mat);
    
private:
    std::vector<Triple> mData;
    Triple getCol(int col) const;
};

#endif /* end of include guard: MATRIX_H_INCLUDED */
