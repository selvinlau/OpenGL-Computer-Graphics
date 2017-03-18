#include <math.h>
#include <iostream>
#include "triple.h"
#include <vector>
#include "matrix3.h"


Matrix3::Matrix3(Triple r1, Triple r2, Triple r3) : mData(ROWS) {
    mData[0] = r1;
    mData[1] = r2;
    mData[2] = r3;
}

Matrix3::Matrix3(const Matrix3 &m) {
    
    mData[0] = m(0);
    mData[1] = m(1);
    mData[2] = m(2);
}

Triple Matrix3::operator*(const Triple &vector) const
{
    Triple result;
    result.x = mData[0].dot(vector);
    result.y = mData[1].dot(vector);
    result.z = mData[2].dot(vector);
    
    return result;
}

Matrix3 Matrix3::operator*(double k) const
{
    Triple r1 = mData[0] * k;
    Triple r2 = mData[1] * k;
    Triple r3 = mData[2] * k;
    
    return Matrix3(r1, r2, r3);
}

Matrix3 Matrix3::operator+(const Matrix3 &mat) const
{
    Triple r1 = mData[0] + mat(0);
    Triple r2 = mData[1] + mat(1);
    Triple r3 = mData[2] + mat(2);
    
    return Matrix3(r1, r2, r3);
}

Triple Matrix3::operator()(int row) const {
    return mData[row];
}

ostream& operator<<(ostream& s, const Matrix3& mat)
{
    return s << endl << mat(0) << endl << mat(1) << endl << mat(2) << endl;
}
