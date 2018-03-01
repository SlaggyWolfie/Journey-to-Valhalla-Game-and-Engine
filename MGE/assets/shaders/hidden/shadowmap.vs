//SHADOW MAP VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}  