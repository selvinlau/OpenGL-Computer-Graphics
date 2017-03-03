//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "renderMode.h"
#include "camera.h"

class Scene
{
private:
    //Added a variable to store the camera model
    int cameraModel; //0 for eye 1 for extended
    
    //Added a camera for the extended camera model
    Camera *camera;
    //Added renderMode
    RenderMode renderMode;
    //Added shadows
    bool shadows;
    
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    
    Color tracePhong(Material *material, Point hit, Vector N, Vector V);
    Color traceNormalBuffer(Vector N);
    Color traceZBuffer(Hit N);
    
    //New methods added
    bool isShadow(const Point hit, Vector L);
    Color reflect(Vector N, Vector R, Point hit, double ks);
    Color sampleColor(Point center);
    
    double minDist;
    double maxDist;
    
    //Added variable to know the recursion depth of the reflections
    int reflectionDepth;
    //Added value to store the maximum number of recursions
    int MAX_REFLECTION_DEPTH;
    //Added value to store the number of samples per pixel
    int numSamples;
public:
    Color trace(const Ray &ray);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setMinMax(int w, int h);
    void setEye(Triple e);
    //Added method to set the render mode
    void setRenderMode(RenderMode rm);
    //Added method to set whether there are shadows or not
    void setShadows(bool b);
    //Added method to check the recursion depth of the reflections
    void setReflectionDepth(int d);
    //Added method to check the number of samples per pixel
    void setNumSamples(int s);
    void setCamera(Camera *c);
    void setCameraModel(int m);
    
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
