//LIT COLOR FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

struct DirectionalLight
{
	vec3 color;
	vec3 direction;
	
	float intensity;
};

struct PointLight
{
	vec3 color;
	vec3 position;
	
	float intensity;
	float range;
};

struct SpotLight
{
	vec3 color;
	vec3 direction;
	vec3 position;

	float fallOff;
	float fallOffOuter;
	
	float intensity;
	float range;
};

struct Light
{
	vec3 ambientLightColor;
	float ambientStrength;
	vec3 ambientTerm;
	
	vec3 attenuationConstants;
	
	int directionalLightsAmount;
	int pointLightsAmount;
	int spotLightsAmount;
};

struct Material
{
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 emissionColor;
	
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
	
	float shininess;
	
	int useDiffuseMap;
	int useSpecularMap;
	int useEmissionMap;
	int useEmission;
};

#define MAX_LIGHTS 32
uniform DirectionalLight directionalLights[MAX_LIGHTS / 4];
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];

uniform Light light;
uniform Material material;

uniform vec3 cameraPosition;

in vec3 worldNormal;
in vec3 worldPosition;
in vec2 textureCoordinate;

out vec4 finalColor;

vec3 GetDirectionalLight(vec3 diffuseColor, vec3 specularColor,
 DirectionalLight directional, vec3 normal, vec3 viewDirection)
{
	float diffuseIntensity = max (0, dot(-directional.direction, normal));
	float specularIntensity = pow(max(dot(reflect(directional.direction, normal), -viewDirection), 0), shininess);
	
    vec3 diffuseTerm = diffuseIntensity * directional.color * diffuseColor;
	vec3 specularTerm = specularIntensity * directional.color * specularColor;

	return diffuseTerm + specularTerm;
}

vec3 GetPointLight(vec3 diffuseColor, vec3 specularColor,
	PointLight point, vec3 normal, vec3 viewDirection)
{
	vec3 deltaDirection = point.position - worldPosition;
	float diffuseIntensity = max (0, dot(normalize(deltaDirection), normalize (normal)));
	float specularIntensity = pow(max(dot(reflect(-normalize(deltaDirection), normal), -viewDirection), 0), material.shininess);
	
	//Attenuation
	float linearQuadraticSum = light.attenuationConstants[1] + light.attenuationConstants[2];
	if (attenuationConstants[0] + linearQuadraticSum > 0.01f) 
	{
		float distanceFromLight = 0; 
		if (linearQuadraticSum > 0.01f) distanceFromLight = length(deltaDirection) - point.range / 2;
		
		float attenuation = 1 / 
		( light.attenuationConstants[0] 
		+ light.attenuationConstants[1] * distanceFromLight 
		+ light.attenuationConstants[2] * distanceFromLight * distanceFromLight);
		
		diffuseIntensity *= attenuation;
		specularIntensity *= attenuation;
	}
	
    vec3 diffuseTerm = diffuseIntensity * spot.color * diffuseColor;
	vec3 specularTerm = specularIntensity * spot.color * specularColor;
	
	return diffuseTerm + specularTerm;
}

vec3 GetSpotLight(vec3 diffuseColor, vec3 specularColor,
 SpotLight spot, vec3 normal, vec3 viewDirection, float minimumIntensity, float maximumIntensity)
{
	vec3 deltaDirection = spot.position - worldPosition;
	
	//Spotlight/Flashlight calculation
	float diffuseIntensity = 0; 
	
	float incident = dot(normalize(deltaDirection), -spot.direction); //gives a cosine
	float fallOffDelta = spot.fallOff - spot.fallOffOuter;
	diffuseIntensity = clamp((incident - spot.fallOffOuter) / fallOffDelta, minimumIntensity, maximumIntensity); 
		
	float specularIntensity = pow(max(dot(reflect(-normalize(deltaDirection), normal), -viewDirection), 0), shininess);
	
	//Attenuation
	float linearQuadraticSum = attenuationConstants[1] + attenuationConstants[2];
	if (attenuationConstants[0] + linearQuadraticSum > 0.01f) 
	{
		float distanceFromLight = 0; 
		if (linearQuadraticSum > 0.01f) distanceFromLight = length(deltaDirection) - point.range / 2;
		
		float attenuation = 1 / 
		( attenuationConstants[0] 
		+ attenuationConstants[1] * distanceFromLight 
		+ attenuationConstants[2] * distanceFromLight * distanceFromLight);
		
		diffuseIntensity *= attenuation;
		specularIntensity *= attenuation;
	}
	
	specularIntensity *= diffuseIntensity;
	
    vec3 diffuseTerm = diffuseIntensity * spot.color * diffuseColor;
	vec3 specularTerm = specularIntensity * spot.color * specularColor;
	
	return diffuseTerm + specularTerm;
}

void main (void) 
{
	vec3 diffuseColor = vec3(0);
	float alpha = 0;
	
	if (material.useDiffuseMap)
	{
		vec4 diffuseTexture = texture2D(material.diffuseMap, textureCoordinate);
		diffuseColor = diffuseTexture.rgb;
		alpha = diffuseTexture.a;
	}
	else
	{
		diffuseColor = material.diffuseColor;
		alpha = 1;
	}
	
	vec3 specularColor = vec3(0);
	
	if (material.useSpecularMap)
		specularColor = texture2D(material.specularMap, textureCoordinate).rgb;
	else
		specularColor = material.specularColor;
	
	vec3 emissionColor = vec3(0);
	
	if (material.useEmission)
	{
		if (material.useEmissionMap)
			emissionColor = texture2D(material.emissionMap, textureCoordinate).rgb;
		else
			emissionColor = material.emissionColor;
	}
	
    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(worldPosition - cameraPosition);

	vec3 result = light.ambientTerm * diffuseColor;
	
    //Directional light(s) - usually one
    for(int i = 0; i < material.directionalLightsAmount; i++) 
		result += GetDirectionalLight(diffuseColor, specularColor, directionalLights[i], normal, viewDirection);
	
    //Point lights
    for(int j = 0; j < material.pointLightsAmount; j++) 
		result += GetPointLight(diffuseColor, specularColor, pointLights[j], normal, viewDirection);   
	
    //Spot lights
    for(int k = 0; k < material.spotLightsAmount; k++) 
		result += GetSpotLight(diffuseColor, specularColor, spotLights[k], normal, viewDirection, light.ambientStrength, 1); 
	
	result += emissionColor;
	
    finalColor = vec4(result, alpha);
}
