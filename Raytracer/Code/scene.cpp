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
#include "renderMode.h"
#include "math.h"

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
            resultColor = tracePhong(material, hit, N, V);
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

Color Scene::tracePhong(Material *material, Point hit, Vector N, Vector V) {
    
    Color resultColor; // place holder
    Color ambient;
    Color diffuse(0.0, 0.0, 0.0);
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
    
        diffuse += max(0.0, NdotL) * lights[i]->color;
        specular += pow(max(0.0, RdotV), material->n) * lights[i]->color;
    }
    
    //Compute the reflection and set the counter to 0
    Color reflection = reflect(N, V, hit, material->ks);
    reflectionDepth = 0;
    
    //Compute the total color
    ambient = material->color * material->ka;
    diffuse = diffuse * material->kd * material->color;
    specular = (specular + reflection) * material->ks;
    
    resultColor = ambient + diffuse + specular + reflection;
    return resultColor;
   
}

Color Scene::reflect(Vector N, Vector V, Point hit, double ks) {
    Color r(0.0, 0.0, 0.0);
    
    if (reflectionDepth < MAX_REFLECTION_DEPTH && ks > 0) {
        Vector VR = 2 * N.dot(V) * N - V;
        Ray l(hit, VR);
        reflectionDepth++;
        r = trace(l) * ks;
    }
    return r;
}

bool Scene::isShadow(const Point hit, Vector L) {
    if (shadows) {
        Ray l(hit, L);
        for (unsigned int i = 0; i < objects.size(); i++) {
        
            Hit hit(objects[i]->intersect(l));
        
            if (!isnan(hit.t)) {
                return true;
            }
        }
    }
    return false;
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

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    
    //If the render mode is zbuffer, we compute the min and max distances
    if(renderMode == ZBUFFER){
        setMinMax(w, h);
    }
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x,y) = col;
        }
    }
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

void Scene::setRenderMode(RenderMode rm)
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
