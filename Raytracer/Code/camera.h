//
// Class that models a camera object of the extended camera model
//

#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "triple.h"

class Camera
{
public:
    Triple eye;
    Vector center;
    Vector up;
    int height;
    int width;
    double pixelSize;
    
    Camera() { }

    void setPixelSize() { pixelSize = up.length(); }
};

#endif /* end of include guard: CAMERA_H_INCLUDED */
