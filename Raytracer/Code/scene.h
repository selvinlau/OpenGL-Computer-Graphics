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
#include <map>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "camera.h"
#include "gooch.h"

class Scene
{
public:
    
    //======================================CONSTANTS==============================================
    //Render mode enum type
    enum RenderModes {
        PHONG,
        ZBUFFER,
        NORMAL,
        GOOCH
    };
    
    typedef map<string, RenderModes> RenderMap;
    static RenderMap mapStringToRender;
    
    //Camera models enum type
    enum CameraModels {
        EYE,
        EXTENDED
    };
    
    typedef map<string, CameraModels> CameraMap;
    static CameraMap mapStringToCamera;
    
    //Default constants
    static const RenderModes DEFAULT_RENDER_MODE = PHONG;
    static const bool DEFAULT_SHADOWS = false;
    static const unsigned int DEFAULT_REFLECTION_DEPTH = 0;
    static const unsigned int DEFAULT_REFRACTION_DEPTH = 0;
    static const int DEFAULT_REFRACT = 1;
    static const unsigned int DEFAULT_SAMPLES = 1;
    static const CameraModels DEFAULT_CAMERA_MODEL = EYE;

    //=============================================================================================
    
    Color trace(const Ray &ray);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    
    //Render was changed, so now the image is created and returned instead of passed as parameter
    Image render();
    
    //Method to aid computing the z-buffer
    void setMinMax(int w, int h);
    
    //New setter methods
    void setRenderMode(RenderModes rm);
    void setShadows(bool b);
    void setReflectionDepth(int d);
    void setRefractionDepth(int d);
    void setRefract(double r);
    void setNumSamples(int s);
    void setCameraModel(CameraModels m);
    void setCamera(Camera *c);
    void setGooch(Gooch *g);
    
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
    
private:
    //Map initializations
    static RenderMap initRenderMap();
    static CameraMap initCameraMap();
    
    //Scene configuration
    RenderModes renderMode;
    bool shadows;
    //reflectionDepth contains the number of reflections done so far, MAX_REFLECTION_DEPTH
    //the maximum number of them
    int reflectionDepth;
    int MAX_REFLECTION_DEPTH;
    //Same as with the reflection variables
    int refractionDepth;
    int MAX_REFRACTION_DEPTH;
    int numSamples;
    CameraModels cameraModel;
    //Refraction coefficient
    double refractCoeff;
    
    //Objects
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    //Added min and max distance for the z-buffer
    double minDist;
    double maxDist;
    //Added a camera for the extended camera model
    Camera *camera;
    //Added gooch model params
    Gooch *gooch;
    
    //Methods
    Color traceColor(RenderModes rm, Material *material, Point hit, Vector N, Vector V);
    Color traceNormalBuffer(Vector N);
    Color traceZBuffer(Hit N);
    
    Color phongDiffuseColor(double NdotL, Color lColor);
    Color phongTotalColor(Color diff, Color matColor, double ka, double kd);
    Color goochColor(Gooch *gooch, double NdotL, double VdotN, Color lColor, Color matColor, double matkd );




    //Check if there is a shadow at the point hit
    bool isShadow(const Point hit, Vector L);
    //Returns the reflection color
    Color reflect(Vector N, Vector V, Point hit, double ks);
    //Returns the refraction color
    Color refract(Vector N, Vector V, Point hit, double eta);
    //Returns the average color of a pixel after tracing numSamples rays through it
    Color sampleColor(Triple eye, Point center, double pixelSize);
    
    //Added methods to render the image
    void renderEyeModel(Image &img, int h, int w);
    void renderExtendedModel(Image &img, int h, int w);
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
