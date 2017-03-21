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
#include <string.h>
#include "tetrahedron.h"
#include "sierpinski.h"
#include "glm.h"
#include "model.h"

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

//Map initialization
Raytracer::ObjectMap Raytracer::mapStringToObject = Raytracer::initObjectMap();

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
    node["viewSize"][0] >> c->width;
    node["viewSize"][1] >> c->height;
    c->setPixelSize();
    
    return c;
}

Gooch* Raytracer::parseGooch(const YAML::Node& node)
{
    double b;
    double y;
    double alpha;
    double beta;
    double edge;
    
    const YAML :: Node * params = node.FindValue("GoochParameters");
    (*params)["b"] >> b;
    (*params)["y"] >> y;
    (*params)["alpha"] >> alpha;
    (*params)["beta"] >> beta;
    
    if( const YAML :: Node * edges = (*params).FindValue("edge")) {
        (*edges) >> edge;
    } else {
        edge = Gooch::DEFAULT_EDGE;
    }
    
    return new Gooch(b, y, alpha, beta, edge);
}

Scene::CameraModels Raytracer::parseCameraModel(const YAML::Node& node) {
    Scene::CameraModels mode;
    
    if( node.FindValue("Camera")) {
        mode = Scene::EXTENDED; //Extended camera
    } else {
        mode = Scene::DEFAULT_CAMERA_MODEL; //Eye
    }
    
    return mode;
}

int Raytracer::parseNumSamples(const YAML::Node& node) {
    int sampleNum;
    
    if( const YAML :: Node * sample = node.FindValue("SuperSampling")) {
        (*sample)["factor"] >> sampleNum;
    } else {
        sampleNum = Scene::DEFAULT_SAMPLES;
    }
    
    return sampleNum;
}

int Raytracer::parseReflectionDepth(const YAML::Node& node) {
    int depthNum;
    
    if( const YAML :: Node * depth = node.FindValue("MaxRecursionDepth")) {
        *depth >> depthNum;
    } else {
        depthNum = Scene::DEFAULT_REFLECTION_DEPTH;
    }
    return depthNum;
}

double Raytracer::parseRefract(const YAML::Node& node) {
    double refract;
    
    if( const YAML :: Node * ref = node.FindValue("refrct")) {
        *ref >> refract;
    } else {
        refract = Scene::DEFAULT_REFRACTION_DEPTH;
    }
    return refract;
}

int Raytracer::parseRefractionDepth(const YAML::Node& node) {
    int depthNum;
    
    if( const YAML :: Node * depth = node.FindValue("MaxRefractionDepth")) {
        *depth >> depthNum;
    } else {
        depthNum = Scene::DEFAULT_REFRACTION_DEPTH;
    }
    return depthNum;
}

//Store the render mode of an image. If there is no mode specified, default to Phongs model.
Scene::RenderModes Raytracer::parseRenderMode(const YAML::Node& node)
{
    Scene::RenderModes resultMode;
    
    if( const YAML :: Node * render = node.FindValue("RenderMode")) {
        std::string mode;
        *render >> mode;
        resultMode = Scene::mapStringToRender[mode];
        
    } else {
        resultMode = Scene::DEFAULT_RENDER_MODE;
    }
    
    return resultMode;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    
    //Parse material texture or color
    if(const YAML :: Node * texture = node.FindValue("texture")) {
        std::string text;
        *texture >> text;
        const char * c = text.c_str();
        m->texture = new Image(c);
    } else {
        node["color"] >> m->color;
        m->texture = NULL;
    }
    
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    
    //Parse refraction coefficient
    if( const YAML :: Node * eta = node.FindValue("eta")) {
        *eta >> m->eta;
    } else {
        m->eta = Material::DEFAULT_ETA;
    }
    return m;
}

Sphere* Raytracer::parseSphere(const YAML::Node& node) {
    Sphere *sphere;
    Point pos;
    double r;
    
    node["position"] >> pos;
    
    if (node["radius"].size() == 2) {
        Vector axis;
        double angle;
        
        node["radius"][0] >> r;
        node["radius"][1] >> axis;
        node["angle"] >> angle;
        
        sphere = new Sphere(pos, r, axis, angle);
    } else {
        node["radius"] >> r;
        sphere = new Sphere(pos, r);
    }
    
    return sphere;
}

Plane* Raytracer::parsePlane(const YAML::Node& node) {
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
    
    return plane;
}

Triangle* Raytracer::parseTriangle(const YAML::Node& node) {
    Point v1;
    Point v2;
    Point v3;
    
    node["v1"] >> v1;
    node["v2"] >> v2;
    node["v3"] >> v3;
    return new Triangle(v1,v2,v3);
}

Cylinder* Raytracer::parseCylinder(const YAML::Node& node) {
    Point p1;
    Point p2;
    double r;
    
    node["p1"] >> p1;
    node["p2"] >> p2;
    node["r"] >> r;
    return new Cylinder(p1,p2,r);
}

Tetrahedron* Raytracer::parseTetrahedron(const YAML::Node& node) {
    Point center;
    double side;
    node["center"] >> center;
    node["side"] >> side;
    return new Tetrahedron(center, side);
}

Sierpinski* Raytracer::parseSierpinski(const YAML::Node& node) {
    Point center;
    double side;
    int recursions;
    
    node["center"] >> center;
    node["side"] >> side;
    node["recursions"] >> recursions;
    return new Sierpinski(center, side, recursions);
}

Model* Raytracer::parseModel(const YAML::Node& node) {
    
    std::string text;
    Point pos;
    double scale;
    
    node["file"] >> text;
    node["position"] >> pos;
    node["scale"] >> scale;
   
    const char* c = text.c_str();
    GLMmodel *model = glmReadOBJ(c);
    
    float* vertices = model->vertices;
    GLMtriangle* triangles = model->triangles;
    unsigned int numTriangles = model->numtriangles;
    
    return new Model(numTriangles, triangles, vertices, pos, scale);
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectSt;
    node["type"] >> objectSt;

    switch (mapStringToObject[objectSt]) {
        case SPHERE:
            returnObject = parseSphere(node);
            break;
        case PLANE:
            returnObject = parsePlane(node);
            break;
        case TRIANGLE:
            returnObject = parseTriangle(node);
            break;
        case CYLINDER:
            returnObject = parseCylinder(node);
            break;
        case TETRAHEDRON:
            returnObject = parseTetrahedron(node);
            break;
        case SIERPINSKI:
            returnObject = parseSierpinski(node);
            break;
        case MODEL:
            returnObject = parseModel(node);
            break;
        default:
            cout << "Shape type " << objectSt << " not recognized." << endl;
    }

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
        returnObject->material->object = returnObject;
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
    bool result;
    
    if( const YAML :: Node * shadows = node.FindValue("Shadows")) {
        *shadows >> result;
    } else {
        result = Scene::DEFAULT_SHADOWS;
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
            Scene::RenderModes rm = parseRenderMode(doc);
            scene->setRenderMode(rm);
            
            scene->setShadows(parseShadows(doc));
            scene->setReflectionDepth(parseReflectionDepth(doc));
            scene->setRefract(parseRefract(doc));
            scene->setRefractionDepth(parseRefractionDepth(doc));
            scene->setNumSamples(parseNumSamples(doc));
            
            //Read and parse the camera model
            if (parseCameraModel(doc) == Scene::EYE) {
                scene->setCameraModel(Scene::EYE);
                scene->setEye(parseTriple(doc["Eye"]));
            } else {
                scene->setCameraModel(Scene::EXTENDED);
                scene->setCamera(parseCamera(doc["Camera"]));
            }
            
            if (rm == Scene::GOOCH) {
                scene->setGooch(parseGooch(doc));
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
    
    cout << "Tracing..." << endl;
    //The image is created in scene to be able to configure its width and height properly
    Image img = scene->render();
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}

//Map initialization
Raytracer::ObjectMap Raytracer::initObjectMap() {
    Raytracer::ObjectMap map;
    
    map["sphere"] = SPHERE;
    map["plane"] = PLANE;
    map["triangle"] = TRIANGLE;
    map["cylinder"] = CYLINDER;
    map["tetrahedron"] = TETRAHEDRON;
    map["sierpinski"] = SIERPINSKI;
    map["model"] = MODEL;
    return map;
}
