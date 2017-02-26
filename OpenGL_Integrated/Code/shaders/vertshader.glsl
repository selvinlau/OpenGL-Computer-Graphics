#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertColor_in;
layout (location = 2) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader

// uniform mat4 modelTransform; for example

uniform mat3 normal;
uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;


// Specify the outputs of the vertex shader
// These will be the input for the fragment shader

// out vec3 vertPos; for example
out vec3 vertColor;
out vec3 coordinates;
out vec3 normalValue;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader

    //Applying the changes to the vertex Coordinates
    gl_Position = proj * view * model * vec4(vertCoordinates_in, 1.0);
    vertColor = vertColor_in;
    coordinates = vertCoordinates_in;
    normalValue = normal * vertNormal_in ;
}
