//
//  Framework for a raytracer
//  File: raytracer.cpp
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

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "cylinder.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>
#include "renderMode.h"
#include <string.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

Camera* Raytracer::parseCamera(const YAML::Node& node)
{
    Camera *c = new Camera();
    node["eye"] >> c->eye;
    node["center"] >> c->center;
    node["up"] >> c->up;
    node["viewSize"][0] >> c->height;
    node["viewSize"][1] >> c->width;
    return c;
}

int Raytracer::parseCameraModel(const YAML::Node& node) {
    int mode = 0; //Eye default
    if( node.FindValue("Camera")) {
        mode = 1; //Extended camera
    }
    
    return mode;
}

int Raytracer::parseNumSamples(const YAML::Node& node) {
    int sampleNum = 1;
    if( const YAML :: Node * sample = node.FindValue("SuperSampling")) {
        (*sample)["factor"] >> sampleNum;
    }
    
    return sampleNum;
}

int Raytracer::parseReflectionDepth(const YAML::Node& node) {
    int depthNum = 0;
    
    if( const YAML :: Node * depth = node.FindValue("MaxRecursionDepth")) {
        *depth >> depthNum;
    }
    return depthNum;
}

//Store the render mode of an image. If there is no mode specified, default to Phongs model.
RenderMode Raytracer::parseRenderMode(const YAML::Node& node)
{
    RenderMode resultMode = PHONG;
    
    if( const YAML :: Node * render = node.FindValue("RenderMode")) {
        std::string mode;
        *render >> mode;
        
        if (mode == "normal") {
            resultMode = NORMAL;
        } else if (mode == "zbuffer") {
            resultMode = ZBUFFER;
        }
    }
    return resultMode;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    node["color"] >> m->color;	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);		
        returnObject = sphere;
        
    } else if (objectType == "plane") {
        Plane *plane;
        std::string form;
        node["formula"] >> form;
        if (form == "points") {
            Point p1;
            Point p2;
            Point p3;
            
            node["p1"] >> p1;
            node["p2"] >> p2;
            node["p3"] >> p3;
            plane = new Plane(p1, p2, p3);
        } else {
            Point point;
            Vector N;
            
            node["point"] >> point;
            node["normal"] >> N;
            plane = new Plane(point, N);
        }
        returnObject = plane;
        
    } else if(objectType == "triangle") {
        Point v1;
        Point v2;
        Point v3;
        
        node["v1"] >> v1;
        node["v2"] >> v2;
        node["v3"] >> v3;
        Triangle *triangle = new Triangle(v1,v2,v3);
        returnObject = triangle;
        
    } else if(objectType == "cylinder") {
        Point p1;
        Point p2;
        double r;
        
        node["p1"] >> p1;
        node["p2"] >> p2;
        node["r"] >> r;
        Cylinder *cylinder = new Cylinder(p1,p2,r);
        returnObject = cylinder;
    }


    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

bool Raytracer::parseShadows(const YAML::Node& node) {
    bool result = false;
    
    if( const YAML :: Node * shadows = node.FindValue("Shadows")) {
        *shadows >> result;
    }
    return result;
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    cout << "recognized";
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
            
            // Read scene configuration options
            
            //Read and parse the render mode
            scene->setRenderMode(parseRenderMode(doc));
            //Read and parse the shadows
            scene->setShadows(parseShadows(doc));
            //Read and parse the max number of recursions
            scene->setReflectionDepth(parseReflectionDepth(doc));
            //Read and parse the number of samples per pixel
            scene->setNumSamples(parseNumSamples(doc));
            
            //Read and parse the camera model
            if (parseCameraModel(doc) == 0) {
                scene->setEye(parseTriple(doc["Eye"]));
            } else {
                scene->setCamera(parseCamera(doc["Camera"]));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(400,400);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
