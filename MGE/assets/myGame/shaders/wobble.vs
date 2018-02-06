//WOBBLE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	mvpMatrix;
uniform float time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated
out float vtime;

void main( void ){
    gl_Position = mvpMatrix * vec4(vertex 
	* (1.0f + 0.25f * sin(vertex.x + vertex.y + vertex.z + 5.0 * time))
	, 1.0f);
	
	//texture animation
	//rotation
	float angle = mod(time, 6.28f);
	float sinR = sin(angle);
	float cosR = cos(angle);
	//translate UVs to central origin, rotate with a 2x2 matrix and then translate back 
	vec2 coord = vec2(uv - 0.5) * mat2(cosR, sinR, -sinR, cosR);
	coord += 0.5;
	
	texCoord = coord;
	vtime = time;

	// texCoord = uv;
}
