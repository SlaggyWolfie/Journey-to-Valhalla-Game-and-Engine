//DEFAULT VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 mvMatrix;
uniform mat4 vpMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

uniform mat4 lightSpaceMatrix;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexUV;

out vec3 worldNormal;
out vec3 worldPosition;
out vec2 textureCoordinate;
out vec4 lightSpacePosition;

void main() 
{
    gl_Position = mvpMatrix * vec4(vertexPosition, 1);
	
    worldNormal = normalMatrix * vertexNormal;
    // worldNormal = vec3 (modelMatrix * vec4 (vertexNormal, 0));
	worldPosition = vec3(modelMatrix * vec4 (vertexPosition, 1));
	
	textureCoordinate = vertexUV;
	
	lightSpacePosition = lightSpaceMatrix * vec4(worldPosition, 1);
}


