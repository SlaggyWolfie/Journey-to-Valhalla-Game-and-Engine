//SHADOW MAP VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec3 vertexNormal;
layout (location=2) in vec2 vertexUV;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}  