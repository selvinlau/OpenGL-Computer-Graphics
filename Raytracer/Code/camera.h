//
// Class that models a camera object of the extended camera model
//

#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "triple.h"

class Camera
{
public:
    Vector eye;        
    Vector center;
    Vector up;
    double height;
    double width;

    Camera() { }

};

#endif /* end of include guard: CAMERA_H_INCLUDED */
