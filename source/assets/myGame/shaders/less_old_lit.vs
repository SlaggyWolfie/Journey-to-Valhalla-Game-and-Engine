//LIT COLOR VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform int vertexLit;
uniform int lightType;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightDirection;
uniform vec3 lightPosition;

uniform float fallOff;
uniform float fallOffOuter;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 worldNormal;
out vec3 worldPosition;
out float diffuseIntensityFromVertex;


// void GetDiffuseIntensity(out float diffuseIntensity)
// {
	// diffuseIntensity = 0;
	// vec3 direction = delta;
	
	// if (vertexLit == 0)
	// {
		// if (lightType == 0) direction = -lightDirection;
		// else if (lightType == 1) diffuseIntensity = max (0, dot(normalize(direction), normalize (worldNormal)));
		// else if (lightType == 2) 
		// {
			// float angle = dot(-normalize(direction), -lightDirection);
			// if (angle > fallOff) diffuseIntensity = 0.8f;
		// }
	// }
// } 

void GetDiffuseIntensity(out float diffuseIntensity)
{
	diffuseIntensity = 0;
	vec3 delta = vec3(0,0,0);
	
	if (vertexLit != 0)
	{
		switch (lightType)
		{
			//Directional Light
			case 0:
			{
				delta = -lightDirection;
				diffuseIntensity = max (0, dot(normalize(delta), normalize (worldNormal)));
				break;
			}
			//Point Light
			case 1:
			{
				delta = lightPosition - worldPosition;
				diffuseIntensity = max (0, dot(normalize(delta), normalize (worldNormal)));
				break;
			}
			//Spotlight/Flashlight
			case 2:
			{
				delta = lightPosition - worldPosition;
				float angle = dot(-normalize(delta), -lightDirection);
				// if (angle > fallOff) diffuseIntensity = 0.8f;
				float fallOffDelta = fallOff - fallOffOuter;
				diffuseIntensity = clamp((incidentAngle - fallOffOuter) / fallOffDelta, 0.0f, 0.8f);   
				break;
			}
		};
	}
} 

void main (void) {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex,1);
	
    //make sure normal is in same space as light direction. You should be able to explain the 0.
    worldNormal = vec3 (modelMatrix * vec4 (normal, 0));
	worldPosition = vec3(modelMatrix * vec4(vertex, 1));
	
    //take the dot of the direction from surface to light with the world surface normal
	diffuseIntensityFromVertex = 0;
	GetDiffuseIntensity(diffuseIntensityFromVertex);
}


