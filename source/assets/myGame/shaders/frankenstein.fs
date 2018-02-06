//LIT TERRAIN (FRANKENSTEIN) FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

struct DirectionalLight
{
	vec3 color;
	vec3 direction;
	// float intensity;
};

struct PointLight
{
	vec3 color;
	vec3 position;
	// float intensity;
};

struct SpotLight
{
	vec3 color;
	vec3 direction;
	vec3 position;

	float fallOff;
	float fallOffOuter;
	
	// float intensity;
};

uniform int numberOfDirectionalLights;
uniform int numberOfPointLights;
uniform int numberOfSpotLights;

//#define MAX_LIGHTS = 20;
uniform DirectionalLight directionalLights[20];
uniform PointLight pointLights[20];
uniform SpotLight spotLights[20];

// uniform int vertexLit;

uniform sampler2D diffuseTexture0;
uniform sampler2D diffuseTexture1;
uniform sampler2D diffuseTexture2;
uniform sampler2D diffuseTexture3;

// uniform sampler2D heightmap;
uniform sampler2D splatmap;

uniform float terrainHeight;
uniform float terrainWidth;
uniform float terrainLength;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

uniform float time;

uniform vec3 ambientTerm;
uniform float ambientContribution;
uniform vec3 attenuationConstants;

uniform vec3 cameraPosition;

in vec3 worldNormal;
in vec3 worldPosition;
in vec2 texCoord;

out vec4 sColor;

vec3 GetDirectionalLight(vec3 diffuseTextureColor, DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	float diffuseIntensity = max (0, dot(-light.direction, normal));
	float specularIntensity = pow(max(dot(reflect(light.direction, normal), -viewDirection), 0), shininess);
	
    vec3 diffuseTerm = diffuseIntensity * light.color * diffuseTextureColor;
	vec3 specularTerm = specularIntensity * light.color * specularColor;
	specularTerm = vec3(0,0,0);
	
	return /*ambientTerm + */diffuseTerm + specularTerm;
}

vec3 GetPointLight(vec3 diffuseTextureColor, PointLight light, vec3 normal, vec3 viewDirection)
{
	vec3 deltaDirection = light.position - worldPosition;
	float diffuseIntensity = max (0, dot(normalize(deltaDirection), normalize (normal)));
	float specularIntensity = pow(max(dot(reflect(-normalize(deltaDirection), normal), -viewDirection), 0), shininess);
	
	//Attenuation
	float linearQuadraticSum = attenuationConstants[1] + attenuationConstants[2];
	if (attenuationConstants[0] + linearQuadraticSum > 0.01f) 
	{
		float distanceFromLight = 0; 
		if (linearQuadraticSum > 0.01f) distanceFromLight = length(deltaDirection);
		
		float attenuation = 1 / 
		( attenuationConstants[0] 
		+ attenuationConstants[1] * distanceFromLight 
		+ attenuationConstants[2] * distanceFromLight * distanceFromLight);
		
		//cannot edit uniforms - still possible to get the results if I wanted
		//ambientTerm *= attenuation;
		diffuseIntensity *= attenuation;
		specularIntensity *= attenuation;
	}
	
    vec3 diffuseTerm = diffuseIntensity * light.color * diffuseTextureColor;
	vec3 specularTerm = specularIntensity * light.color * specularColor;
	
	return /*ambientTerm + */diffuseTerm + specularTerm;
}

vec3 GetSpotLight(vec3 diffuseTextureColor, SpotLight light, vec3 normal, vec3 viewDirection, float minimumIntensity, float maximumIntensity)
{
	vec3 deltaDirection = light.position - worldPosition;
	
	//Spotlight/Flashlight calculation
	float diffuseIntensity = 0; 
	// if (dot(normalize(deltaDirection), normalize (normal)) > -0.1f) 
	if (true) 
	{
		float incident = dot(normalize(deltaDirection), -light.direction); //gives a cosine
		float fallOffDelta = light.fallOff - light.fallOffOuter;
		diffuseIntensity = clamp((incident - light.fallOffOuter) / fallOffDelta, minimumIntensity, maximumIntensity); 
	}
	float specularIntensity = pow(max(dot(reflect(-normalize(deltaDirection), normal), -viewDirection), 0), shininess);
	
	specularIntensity *= diffuseIntensity;
	
    vec3 diffuseTerm = diffuseIntensity * light.color * diffuseTextureColor;
	vec3 specularTerm = specularIntensity * light.color * specularColor;
	
	return /*ambientTerm + */diffuseTerm + specularTerm;
}

//Water
vec2 AnimateUV(vec2 uv)
{
	//rotation
	float angle = mod(time * 0.1f, 6.28f);
	float sinR = sin(angle*3);
	float cosR = cos(angle);
	//translate UVs to central origin, rotate with a 2x2 matrix and then translate back 
	vec2 coord = vec2(uv - 0.5) * mat2(cosR, sinR, -sinR, cosR);
	coord += 0.5;
	
	return coord;
}

vec4 GetMonoPlanarTextureMappingColor(vec2 uv, float scale)
{
	vec4 splatColor = texture2D(splatmap, texCoord);
	vec4 color = 
		splatColor.r * texture2D(diffuseTexture0, uv * scale) +
		splatColor.g * texture2D(diffuseTexture1, AnimateUV(uv) * scale) +
		splatColor.b * texture2D(diffuseTexture2, uv * scale) +
		splatColor.a * texture2D(diffuseTexture3, uv * scale);
	
	color.a = 1;	
	
	return color;
}

vec4 GetTriPlanarTextureMappingColor(vec3 position, vec3 normal, vec3 scale)
{
	vec3 blending = abs(normal);
	blending = normalize(max(blending, 0.00001f));
	float blendingTotal = (blending.x + blending.y + blending.z);
	blending /= blendingTotal;

	vec4 x_axis_color = GetMonoPlanarTextureMappingColor(position.yz, scale.x);
	vec4 y_axis_color = GetMonoPlanarTextureMappingColor(position.xz, scale.y);
	vec4 z_axis_color = GetMonoPlanarTextureMappingColor(position.xy, scale.z);
	vec4 color = x_axis_color * blending.x + y_axis_color * blending.y + z_axis_color * blending.z;
	color.a = 1;
	
	return color;
}

vec4 GetTriPlanarTextureMappingColor_SingularTexture(vec3 position, vec3 normal, vec3 scale)
{
	vec3 blending = abs(normal);
	blending = normalize(max(blending, 0.00001f));
	float blendingTotal = (blending.x + blending.y + blending.z);
	blending /= blendingTotal;
	// blending /= vec3(blendingTotal, blendingTotal, blendingTotal);

	vec4 x_axis_color = texture2D( diffuseTexture0, position.yz * scale.x);
	vec4 y_axis_color = texture2D( diffuseTexture0, position.xz * scale.y);
	vec4 z_axis_color = texture2D( diffuseTexture0, position.xy * scale.z);
	vec4 color = x_axis_color * blending.x + y_axis_color * blending.y + z_axis_color * blending.z;
	color.a = 1;
	
	return color;
}

void main (void) 
{
	// vec4 diffuseTextureColorA = texture2D(diffuseTexture, vec2( 0, 0 ));
	
    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(worldPosition - cameraPosition);
	
	vec4 splatColor = texture2D(splatmap, texCoord);
	vec4 color = GetTriPlanarTextureMappingColor(worldPosition, normal, vec3(1,1,1));
	// vec4 color = GetTriPlanarTextureMappingColor(worldPosition, normal, vec3(0.03f,0.03f,0.03f));
	// vec4 color = GetMonoPlanarTextureMappingColor(texCoord, 1);	

	vec3 result = ambientTerm * vec3(color);
	
    //Directional light(s) - usually one
    for(int i = 0; i < numberOfDirectionalLights; i++) result += GetDirectionalLight(vec3(color), directionalLights[i], normal, viewDirection);
	
    //Point lights
    for(int j = 0; j < numberOfPointLights; j++) result += GetPointLight(vec3(color), pointLights[j], normal, viewDirection);   
	
    //Spot lights
    for(int k = 0; k < numberOfSpotLights; k++) result += GetSpotLight(vec3(color), spotLights[k], normal, viewDirection, ambientContribution, 1);   
	
	sColor = vec4(result, 1);
    // sColor = vec4(0.5,0.5,0.5,1);
	// sColor = diffuseTextureColorA;
}
