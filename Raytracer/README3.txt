RAYTRACER 1
=====================================================================================

ADDED CLASSES
The following classes were included:

->plane.cpp/.h
		Plane class, which is a subclass of the Object class. Represents an infinite 		
		plane in the scene. Can be constructed from the general form (point and 			
		normal) or from 3 points. 
		Usage in the YAML files:
		- type: plane						
		  formula: [general or points]					 
  		  [point:, normal:] or [p1:, p2:, p3:]
		Note: the formula field is mandatory	

->triangle.cpp/.h
		Triangle class, which is a subclass of the Object class. Represents a 				
		triangle in the scene. It is constructed from its 3 vertices. Usage in the 			
		YAML files:	
		- type: triangle				 
  		  v1:
		  v2:
		  v3:
		
->cylinder.cpp/.h
		Cylinder class, which is a subclass of the Object class. Represents a 				
		finite cylinder in the scene. It is constructed from the center of its bottom 		
		cap, the center of its top cap and its radius. It can be constructed then in 		
		any position. Usage in the YAML files:
		- type: cylinder
		  p1: (bottom cap)
		  p2: (top cap)
		  r: (radius)

->renderMode.h
		Enum class to model the different types of render modes.	

MODIFICATIONS

->raytracer.cpp/.h 
		Method parseRenderMode was added in order to read that field and more objects
 		were added to parseObject.

->scene.cpp/.h
		Attributes included:
			 A render mode attribute was added to select which one to use, alongside 			 
				with a min and max distance attributes.
		Methods included:
			->setMinMax: This method goes thru each and every pixel to obtain min and 				
				max distance before rendering z-buffer. 
			->parseRenderMode: Get rendermode from yaml file
			->setRenderMode: Set the render mode for the upcoming object rendering			->tracePhong: Contains algorithm to generate phong
			->traceZBuffer: Contains algorithm to generate zbuffer
			->traceNormalBuffer: Contains algorithm to generate normal buffer

->sphere.cpp/.h
		The intersect method was finished.

=====================================================================================

RAYTRACER 2
=====================================================================================
DELETED CLASSES
->renderMode.h was deleted
	
ADDED CLASSES
->camera.h
			Class that models a camera object of the extended view model. 

MODIFICATIONS
->image.h
			->New constants for the default image height and width were added.
->material.h
			->New default constant for the eta of a material
			->Eta parameter
->raytracer.h
	Attributes included:
			->Objects enum type was added along with an auxiliar type and a map to be
 			able to transform strings to the enum. 

	Methods included:
			->initObjectMap(): initialices the map between the strings and the Object
								enum
			->Parsing methods to read the values from the yaml files:
								-parseShadows
								-parseReflectionDepth
								-parseNumSamples
								-parseCamera
								-parseCameraModel

->raytracer.cpp
			->Object enum type map initialisation method and call added
			->Parse methods changed to use scene and raytracer default and enum
			constants, changing if-elses for switch statements when possible
			->Parse method calls added to read scene
			->renderToFile() no longer creates the image object, is created in scene
			instead, in order to be able to get the height and with values from its
			camera object 	 				
					
->scene.h	
	Attributes included:
			->RenderModes enum type was added along with an auxiliar type and a map 
			to be able to transform strings to the enum.			 
			->CameraModels enum type was added along with an auxiliar type and a map 
			to be able to transform strings to the enum.
			->New constants for the default render mode, shadows, reflection depth, 
			refraction depth, refraction coefficient, samples and camera model were
			added.
			->A shadows boolean to specify whether to check for shadows or not was 
			added
			->reflection depth and maximum reflection depth were added. The first one
			holds the current number of reflections and the maximum contains how many
			should be done.
			->refraction depth and maximum refraction depth were added. The first one
			holds the current number of refractions and the maximum contains how many
			should be done.
			->refractCoeff stores the refraction coefficient of the scene
			->numSamples was added to store the number of rays to shoot through each
			pixel
			->A CameraModel variable was added to store whether the model used is the 
			extended one or not
			->A camera object was added

	Methods included:	
			->Setter methods to store the values from the yaml files:
								-setShadows
								-setReflectionDepth
								-setRefractionDepth
								-setRefract
								-setNumSamples
								-setCamera
								-setCameraModel	

			->isShadow():		Returns true if a point is in shadow
			->reflect():		Returns the color of the reflection of a point
			->refract():		Returns the color of the refraction of a point
			->sampleColor():	Returns the average color of a pixel after tracing
								numSamples rays through it
 			->renderEyeModel() 
			and renderExtendedModel():
								The render() method now is split and calls each of this
								methods according to the CameraModel. renderEyeModel()
								does the same it did the render method previously,
								renderExtendedModel() uses the camera object attributes
								in order to render the image.
			
->scene.cpp
		->RenderModes and CameraModels enum types map initialisation methods and 
		calls added	   
		->Methods changed to use scene default and enum constants, changing if-elses
		for switch statements when possible.
		->tracePhong():
								Now calls the method reflect
		->render(): 
								No longer takes an image as parameter, instead it is
								created and returned. This is in order to choose the
								width and height of the image.
		->isShadow(), reflect(), refract(), sampleColor(), renderEyeModel() and 
		renderExtendedModel() were implemented.

=====================================================================================

RAYTRACER 3
=====================================================================================
ADDED CLASSES
->composite.h
					Class that models a shape which is made of an array of other shapes of 
					the same type. 

->gooch.h			Class that stores the Gooch model params
					Usage in the YAML files:
						-b: double
						-y: double
						-alpha: double
						-beta: double
						-edge: double
					Note: edge is optional, if set it determines the thickness of a black line
					around the edges of shapes.

->glm.h	&			Classes that read a model stored in a .obj file
  glm.c

->matrix3.h &		Classes that model a 3x3 matrix
  matrix3.cpp	

->model.h &			Classes that represent a model comprised of an array of triangles
  model.cpp			Usage in the YAML files:
						-type: model
						-file: file_name.obj
						-position: [x, y, z]
						-scale: double
					All fields are mandatory. Position is the center of
					the model.

->sierpinski.h &	Classes that model a Sierpinski tetrahedron.
  sierpinski.cpp	Usage in the YAML files:
						-type: sierpinski
						-center: [x, y, z]
						-side: double
						-recursions: int
					All fields are mandatory. Recursions indicates the number of times that
					the tetrahedron will be splited.

->tetrahedron.h &	Classes that model a regular tetrahedron.
  tetrahedron.cpp	Usage in the YAML files:
						-type: tetrahedron
						-center: [x, y, z]
						-side: double


MODIFICATIONS
->hit.h				A default constructor was added

->material.h		->Parent object pointer was added
					->Texture object
					->Method getColor was added to retrieve the color of the material at a 
						certain point

->material.cpp		Method getColor was implemented, calling to the parent object to rotate the
						point first.

->object.h			New virtual method textureCoordinates added and implemented in sphere.

->raytracer.h		Parsing methods added and/or modified to read the values from the yaml files

->scene.h			->tracePhong method replaced by traceColor. 
					->traceColor method added, it traces both phong and gooch since they are quite
						similar, changing the diffuse light for the gooch color when appropriate.

					->goochColor, phongTotalColor and phongDiffuseColor added to compute the specified
						colors.

->sphere.h &		->rotation matrix added as an attribute
  sphere.cpp		->rotate method implemented
  					->textureCoordinates method implemented
					
