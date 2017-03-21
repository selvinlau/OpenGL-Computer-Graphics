//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"
#include "camera.h"
#include "math.h"
#include "gooch.h"

//Map initialization
Scene::RenderMap Scene::mapStringToRender = Scene::initRenderMap();
Scene::CameraMap Scene::mapStringToCamera = Scene::initCameraMap();


Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {

        Hit hit(objects[i]->intersect(ray));
        
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }
    
    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector
    
    //Trace the image according to the specified render mode
    Color resultColor;
    switch (renderMode) {
        case PHONG:
        case GOOCH:
            resultColor = traceColor(renderMode, material, hit, N, V);
            break;
        case ZBUFFER:
            resultColor = traceZBuffer(min_hit);
            break;
        case NORMAL:
            resultColor = traceNormalBuffer(N);
            break;
        default:
            break;
    }
    
    return resultColor;
}

Color Scene::goochColor(Gooch *gooch, double NdotL, double VdotN, Color lColor, Color matColor, double matkd ) {
    Color goochColor(0.0, 0.0, 0.0);
    
    if (VdotN < gooch->edge) {
        return Color(0.0, 0.0, 0.0);
    }
    
    Color kd;
    Color kWarm;
    Color kCool;
        
    kd = lColor * matColor * matkd;
    kCool = gooch->kBlue + gooch->alpha * kd;
    kWarm = gooch->kYellow + gooch->beta * kd;
        
    goochColor += kCool * (1 - NdotL) / 2 + kWarm * (1 + NdotL) / 2;
    
    return goochColor;
}

Color Scene::phongDiffuseColor(double NdotL, Color lColor) {
    return max(0.0, NdotL) * lColor;
}

Color Scene::phongTotalColor(Color diff, Color matColor, double ka, double kd) {
    Color ambient = matColor * ka;
    Color diffuse = diff * matColor * kd;
    return ambient + diffuse;
}

Color Scene::traceColor(RenderModes rm, Material *material, Point hit, Vector N, Vector V) {
    Color resultColor; // place holder
    Color color(0.0, 0.0, 0.0);
    Color specular(0.0, 0.0, 0.0);
    
    double NdotL;
    double RdotV;
    
    Vector L;
    Vector R;
    
    //Add each light effect in the diffuse and specular colors
    for (unsigned int i = 0; i < lights.size(); i++) {
        
        L = (lights[i]->position - hit).normalized();
        
        if (isShadow(hit, L)) {
            continue;
        }
        
        NdotL = N.dot(L);
        
        R = (2 * NdotL * N - L).normalized();
        RdotV = R.dot(V);
        
        switch (rm) {
            case PHONG:
                color += phongDiffuseColor(NdotL, lights[i]->color);
                break;
            case GOOCH:
                color += goochColor(gooch, NdotL, V.dot(N), lights[i]->color, material->getColor(hit), material->kd);
                break;
            default:
                break;
        }
        
        specular += pow(max(0.0, RdotV), material->n) * lights[i]->color;
    }
    
    //Compute the reflection and set the counter to 0 after
    Color reflection = reflect(N, V, hit, material->ks);
    reflectionDepth = 0;
    
    //Compute the refraction and set the counter to 0 after
    Color refraction = refract(N, V, hit, material->eta);
    refractionDepth = 0;
    
    
    if (rm == PHONG) {
        color = phongTotalColor(color, material->getColor(hit), material->ka, material->kd);
    }
    
    specular = specular * material->ks;
    
    resultColor = color + specular + reflection + refraction;
    
    return resultColor;
}

Color Scene::traceNormalBuffer(Vector N)
{
    return Color(N / 2 + 0.5);
}

Color Scene::traceZBuffer(Hit min_hit)
{
    //With the Min and Max obtained, we could come out with a fraction to scale the rgb
    double value = (min_hit.t-minDist)/(maxDist-minDist);
    Color resultColor = Color(1-value,1-value,1-value);
    return resultColor;
}

Color Scene::reflect(Vector N, Vector V, Point hit, double ks) {
    Color r(0.0, 0.0, 0.0);
    
    //Check whether to keep the reflections or not
    if (reflectionDepth < MAX_REFLECTION_DEPTH && ks > 0) {
        //Compute the reflection ray and get the color
        Vector VR = 2 * N.dot(V) * N - V;
        Ray l(hit, VR);
        reflectionDepth++;
        r = trace(l) * ks;
    }
    return r;
}

Color Scene::refract(Vector N, Vector V, Point hit, double eta) {
    Color r(0.0, 0.0, 0.0);
    //Check whether to keep the refractions or not
    if (refractionDepth < MAX_REFRACTION_DEPTH && eta > 0) {
        //Compute the refraction ray and get the color
        Vector T = (refractCoeff * (V - V.dot(N) * N)) / eta - N * sqrt(1 - pow(refractCoeff, 2) * (1 - (pow(V.dot(N), 2))) / pow(eta, 2));
        Ray l(hit, -T);
        refractionDepth++;
        r = trace(l) * eta;
    }
    return r;
}

bool Scene::isShadow(const Point hit, Vector L) {
    if (shadows) {
        Ray l(hit, L);
        //Check if the ray hits with another object
        for (unsigned int i = 0; i < objects.size(); i++) {
        
            Hit hit(objects[i]->intersect(l));
        
            if (!isnan(hit.t)) {
                return true;
            }
        }
    }
    return false;
}

Image Scene::render() {
    int w, h;
    
    switch (cameraModel) {
        case EYE:
            h = Image::DEFAULT_IMG_HEIGHT;
            w = Image::DEFAULT_IMG_WIDTH;
            break;
        case EXTENDED:
            w = camera->width;
            h = camera->height;
            break;
    }
    
    Image img(w, h);
    //If the render mode is zbuffer, we compute the min and max distances
    if(renderMode == ZBUFFER){
        setMinMax(w, h);
    }
    
    switch (cameraModel) {
        case EYE:
            renderEyeModel(img, h, w);
            break;
        case EXTENDED:
            renderExtendedModel(img, h, w);
            break;
    }
    
    return img;
}

void Scene::renderEyeModel(Image &img, int h, int w) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            img(x, y) = sampleColor(eye, pixel, 1.0);
        }
    }
}

void Scene::renderExtendedModel(Image &img, int h, int w) {
    
    Vector G = camera->center - camera->eye;        //Gaze vector
    Vector A = G.cross(camera->up).normalized();    //Rigth vector on the eye point
    Vector B = A.cross(G).normalized();             //Up vector on the eye point, coplanar to A and orthogonal to A and G
    
    double totalH = h * camera->pixelSize / 2;      //Half of the total height of the "screen"
    double totalW = w * camera->pixelSize / 2;      //Half of the total width of the "screen"
    
    Vector R = A * totalW;                          //Rigth vector at the center of the image
    Vector U = B * totalH;                          //Up vector at the center of the image
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+(camera->pixelSize / 2), h-1-y+(camera->pixelSize / 2), 0);
            Point P = camera->center + (2 * pixel.x / (w - 1) - 1) * R + (2 * pixel.y / (h - 1) - 1) * U;
            img(x, y) = sampleColor(camera->eye, P, camera->pixelSize);
            
        }
        
    }
}

Color Scene::sampleColor(Triple eye, Point center, double pixelSize) {
    Color totalColor(0.0, 0.0, 0.0);
    //Divide the side size of the pixel between the total number of rays that
    //will go through it
    int numRays = pow(numSamples, 2);
    double shift = pixelSize / numSamples;
    
    //Start tracing rays from the left top corner of the pixel
    //Do an integer division to know how much to shift at the beginning
    Point leftTopCorner(center);
    leftTopCorner.x = center.x - (numSamples / 2) * shift + (shift / 2);
    leftTopCorner.y = center.y + (numSamples / 2) * shift - (shift / 2);
    
    //Iterate through the pixel per colums, right to bottom and left to right
    //(same way that it is done for the pixels of the image in the render method)
    for (unsigned int i = 0; i < numSamples; i++) {
        center.x = leftTopCorner.x + i * shift;
        for (unsigned int j = 0; j < numSamples; j++) {
            center.y = leftTopCorner.y - j * shift;
            //Trace a ray through the new position of the pixel, compute the color
            //of that point and add it to the total
            Ray ray(eye, (center - eye).normalized());
            totalColor += trace(ray);
        }
    }
    //Average the color and return it
    totalColor /= numRays;
    totalColor.clamp();
    return totalColor;
}

void Scene::setMinMax(int w, int h){
    
    //Run through every pixel to get the correct Min and Max
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
            Object *obj = NULL;
            for (unsigned int i = 0; i < objects.size(); ++i) {
                Hit hit(objects[i]->intersect(ray));
                if (hit.t<min_hit.t) {
                    min_hit = hit;
                    obj = objects[i];
                }
            }
            if(minDist==0){
                minDist = min_hit.t;
            }
            if(min_hit.t <minDist && min_hit.t!=0){
                minDist = min_hit.t;
            }
            if(min_hit.t>maxDist && min_hit.t!=std::numeric_limits<double>::infinity()){
                maxDist = min_hit.t;
            }
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

//New setters
void Scene::setRenderMode(RenderModes rm)
{
    renderMode = rm;
}

void Scene::setShadows (bool b)
{
    shadows = b;
}

void Scene::setReflectionDepth(int d) {
    reflectionDepth = 0;
    MAX_REFLECTION_DEPTH = d;
}

void Scene::setRefractionDepth(int d) {
    refractionDepth = 0;
    MAX_REFRACTION_DEPTH = d;
}

void Scene::setRefract(double r) {
    refractCoeff = r;
}

void Scene::setNumSamples(int s) {
    numSamples = s;
}

void Scene::setCamera(Camera *c) {
    camera = c;
}

void Scene::setCameraModel(Scene::CameraModels m) {
    cameraModel = m;
}

void Scene::setGooch(Gooch *g) {
    gooch = g;
}

//Map initializations
Scene::RenderMap Scene::initRenderMap() {
    Scene::RenderMap map;
    
    map["phong"] = PHONG;
    map["z-buffer"] = ZBUFFER;
    map["normal"] = NORMAL;
    map["gooch"] = GOOCH;
    return map;
}

Scene::CameraMap Scene::initCameraMap() {
    Scene::CameraMap map;
    
    map["eye"] = EYE;
    map["Camera"] = EXTENDED;
    return map;
}
