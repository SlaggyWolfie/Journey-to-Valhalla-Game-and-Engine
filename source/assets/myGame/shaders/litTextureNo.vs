//LIT COLOR VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 worldNormal;
out vec3 worldPosition;
out vec2 texCoord;

void main (void) {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex,1);
	
    //make sure normal is in same space as light direction. You should be able to explain the 0.
    worldNormal = vec3 (modelMatrix * vec4 (normal, 0));
	worldPosition = vec3(modelMatrix * vec4(vertex, 1));
	
	texCoord = uv;
}


