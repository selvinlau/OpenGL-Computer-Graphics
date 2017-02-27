#version 330 core

// Define constants
#define M_PI 3.141593
// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertColor;
in vec3 coordinates;
in vec3 normalValue;

// in vec3 vertPos; Using the output from the vertex shader example

// Specify the Uniforms of the vertex shaders
// uniform vec3 lightPosition; for example
uniform vec3 materialColor;
uniform vec4 intensity;
uniform vec3 lightPos;
uniform vec3 position;
uniform vec3 cam;
uniform int setting;
// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    if(setting == 0){
        fColor = vec4(vertColor, 1.0);//for cube
    }
    else{
        vec3 lightColor = vec3(1,1,1);
        vec3 L = normalize(lightPos - position);
        vec3 N = normalize(normalValue);

        vec3 R = reflect(L, N);
        vec3 V = normalize(cam - position);
        vec3 specular = pow(max(0.0,dot(R,V)),intensity.w) * lightColor * intensity.z;
        vec3 ambient = materialColor * intensity.x;
        vec3 diffuse = max(0.0,dot(N,L)) * materialColor * lightColor * intensity.y ;
        diffuse += ambient;
        diffuse += specular;
        fColor = vec4(diffuse,1.0);
    }



}
