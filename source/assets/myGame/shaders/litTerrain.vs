//LIT TERRAIN VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform sampler2D heightmap;

uniform float terrainHeight;
uniform float terrainWidth;
uniform float terrainLength;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 worldNormal;
out vec3 worldPosition;
out vec2 texCoord;

vec3 CalculateSlopedNormal(float offsetAmount)
{
	float offsetX = 1.0f/ textureSize(heightmap,0).x * offsetAmount;
	float offsetY = 1.0f/ textureSize(heightmap,0).y * offsetAmount;

	vec3 min_x = vec3(uv.x - offsetX, 0, uv.y);
	min_x.y = texture2D(heightmap, min_x.xz).y;
	
	vec3 max_x = vec3(uv.x + offsetX, 0, uv.y);
	max_x.y = texture2D(heightmap, max_x.xz).y;
	
	vec3 min_y = vec3(uv.x, 0, (uv.y - offsetY));
	min_y.y = texture2D(heightmap, min_y.xz).y;
	
	vec3 max_y = vec3(uv.x, 0, (uv.y + offsetY));
	max_y.y = texture2D(heightmap, max_y.xz).y;
	
	vec3 min_xScaled = vec3(min_x.x * terrainWidth, min_x.y * terrainHeight, -min_x.z * terrainLength);
	vec3 max_xScaled = vec3(max_x.x * terrainWidth, max_x.y * terrainHeight, -max_x.z * terrainLength);
	vec3 min_yScaled = vec3(min_y.x * terrainWidth, min_y.y * terrainHeight, -min_y.z * terrainLength);
	vec3 max_yScaled = vec3(max_y.x * terrainWidth, max_y.y * terrainHeight, -max_y.z * terrainLength);
	
	return normalize(cross(max_xScaled - min_xScaled, max_yScaled - min_yScaled));
	
	// vec3 off = vec3(1.0, 1.0, 0.0) * offsetAmount;
  // float hL = distance(uv - off.xz);
  // float hR = distance(uv + off.xz);
  // float hD = distance(uv - off.zy);
  // float hU = distance(uv + off.zy);

  // // deduce terrain normal
  // N.x = hL - hR;
  // N.y = hD - hU;
  // N.z = 2.0;
  // N = normalize(N);
}

void main (void) {
	vec4 heightColor = texture(heightmap, uv);
	vec4 newVertex = vec4(vertex.x * terrainWidth, vertex.y + heightColor.y * terrainHeight, vertex.z*terrainLength, 1);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * newVertex;
	
    //make sure normal is in same space as light direction. You should be able to explain the 0.
    worldNormal = vec3 (modelMatrix * vec4 (CalculateSlopedNormal(10), 0));
	worldPosition = vec3(modelMatrix * newVertex);
	texCoord = uv;
}


