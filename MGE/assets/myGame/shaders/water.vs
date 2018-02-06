//WATER TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

float random(vec2 randomInput)
{
	return fract(sin(dot(vec3(randomInput, 0), vec3(18.6521, 88.92822, 27.9281))) * 83521.2354);
}

void main( void ){
	
	vec4 newVertex = vec4(vertex, 1);
	vec4 worldPos = modelMatrix * newVertex;
	worldPos.y += sin(time/**/ + random(vec2(floor(time / 100), mod(time, 100))) / 10 + worldPos.x * worldPos.z/ 16) / 2;
	// gl_Position = mvpMatrix * newVertex;
    gl_Position = projectionMatrix * viewMatrix * worldPos;
	
	//texture animation
	//rotation
	float angle = mod(time * 0.1f, 6.28f);
	float sinR = sin(angle*3);
	float cosR = cos(angle);
	//translate UVs to central origin, rotate with a 2x2 matrix and then translate back 
	vec2 coord = vec2(uv - 0.5) * mat2(cosR, sinR, -sinR, cosR);
	coord += 0.5;
	
	texCoord = coord;

	// texCoord = uv;
}
