
INSTRUCTIONS
-------------------------------------------------------------------------------------1.	Double-click OpenGL_16_17.pro and open project with QTcreator.2.	Access mainview.h and look for the variable setting3.	There will be 2 options available for this setting		a.	setting = 0;//Cube		b.	setting = 1;//Sphere
4.	Build and run to see results		a.	If you were to encounter any issues that prevent you from seeing the
 			 correct result, please clean and build before running the project 
			 again
5.	Renderer and Model Dropdown-list isn’t working
-------------------------------------------------------------------------------------

MODIFICATIONS AND CHANGES
-------------------------------------------------------------------------------------1.	mainview.h		a.	Variables(private)				i.	 GLuint texCoordBO // declared to store the pointer of texture coordinate				ii.	 GLuint texturePtr // pointer to store the loaded Texture				iii. uint animationFrame // used for animating the scene				iv.	 GLint samplerTex // uniform variable used to be passed over to the fragshader
3.	raytracerscene.cpp		a.	renderCube()				i.	 This method is created to rearrange the codes used for generating cube.
-------------------------------------------------------------------------------------
CHALLENGES
-------------------------------------------------------------------------------------
1.	We had alot of issues with binding of the texture. The main challenges of this lab is that 
	we have to read up alot on the perimeters values and understand how texture works.
	In the fragshader, according to the lab, we were asked to use texture2D() but apparently,
	it doesnt work and only texture() works.-------------------------------------------------------------------------------------


Perimeters of Texture
-------------------------------------------------------------------------------------
 GL_NEAREST
 Returns the value of the texture element that is nearest specified texture coordinates.

 GL_LINEAR
 Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
 
 GL_CLAMP_TO_EDGE
 It clamps the image from the edge and starts working in an direction.
 
 GL_CLAMP_TO_BORDER
 Similar manner to GL_CLAMP_TO_EDGE.
 
 GL_REPEAT
 It repeats the texture from the edge.
 
 GL_MIRRORED_REPEAT
 It is similar to repeat, just that it flips the image like a mirror.
	
 
-------------------------------------------------------------------------------------

Mutiplying the Coordinates by [2,2]
-------------------------------------------------------------------------------------
Initially, our texture coordinates is only from 0,0 to 1,1. By multiplying it by 2,2 , our texture will
have a bigger space to work on our model.