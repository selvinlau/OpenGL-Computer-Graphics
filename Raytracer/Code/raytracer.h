//
//  Framework for a raytracer
//  File: raytracer.h
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

#ifndef RAYTRACER_H_6GQO67WK
#define RAYTRACER_H_6GQO67WK

#include <iostream>
#include <string>
#include "triple.h"
#include "light.h"
#include "scene.h"
#include "yaml/yaml.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "cylinder.h"
#include "tetrahedron.h"
#include "sierpinski.h"
#include "camera.h"
#include "gooch.h"
#include "glm.h"
#include "model.h"

class Raytracer {
public:
    //Objects enum type
    enum Objects {
        SPHERE,
        PLANE,
        TRIANGLE,
        CYLINDER,
        TETRAHEDRON,
        SIERPINSKI,
        MODEL
    };
    typedef map<string, Objects> ObjectMap;
    static ObjectMap mapStringToObject;
    
    Raytracer() { }
    
    bool readScene(const std::string& inputFilename);
    void renderToFile(const std::string& outputFilename);
    
private:
    //Map initialization
    static ObjectMap initObjectMap();
    
    Scene *scene;
    // Couple of private functions for parsing YAML nodes
    Material* parseMaterial(const YAML::Node& node);
    Object* parseObject(const YAML::Node& node);
    Light* parseLight(const YAML::Node& node);
    
    //New parsing methods added
    Scene::RenderModes parseRenderMode(const YAML::Node& node);
    bool parseShadows(const YAML::Node& node);
    int parseReflectionDepth(const YAML::Node& node);
    double parseRefract(const YAML::Node& node);
    int parseRefractionDepth(const YAML::Node& node);
    int parseNumSamples(const YAML::Node& node);
    Camera* parseCamera(const YAML::Node& node);
    Scene::CameraModels parseCameraModel(const YAML::Node& node);
    Gooch* parseGooch(const YAML::Node& node);
    
    //Parse objects
    Sphere* parseSphere(const YAML::Node& node);
    Plane* parsePlane(const YAML::Node& node);
    Triangle* parseTriangle(const YAML::Node& node);
    Cylinder* parseCylinder(const YAML::Node& node);
    Tetrahedron* parseTetrahedron(const YAML::Node& node);
    Sierpinski* parseSierpinski(const YAML::Node& node);
    Model* parseModel(const YAML::Node& node);
};

#endif /* end of include guard: RAYTRACER_H_6GQO67WK */
