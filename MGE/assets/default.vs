//LIT COLOR VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mvMatrix;
uniform mat4 vpMatrix;
uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

out vec3 worldNormal;
out vec3 worldPosition;
out vec2 textureCoordinate;

void main() 
{
    gl_Position = mvpMatrix * vec4(vertexPosition, 1);
	
    worldNormal = vec3 (modelMatrix * vec4 (vertexNormal, 0));
	worldPosition = vec3(normalMatrix * vec4 (vertexPosition, 1));
	
	textureCoordinate = vertexUV;
}


