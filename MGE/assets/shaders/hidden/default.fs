//DEFAULT FRAGMENT SHADER
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
	
	bool useDiffuseMap;
	bool useSpecularMap;
	bool useEmissionMap;
	bool useEmission;
};

#define MAX_LIGHTS 32
uniform DirectionalLight directionalLights[MAX_LIGHTS / 4];
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS / 2];

uniform Light light;
uniform Material material;

uniform vec3 cameraPosition;

uniform sampler2D shadowMap;

in vec3 worldNormal;
in vec3 worldPosition;
in vec2 textureCoordinate;
in vec4 lightSpacePosition;

out vec4 finalColor;

//Forward declare methods;
float GetDiffuseIntensity(vec3 direction, vec3 normal);
float GetSpecularIntensity(vec3 direction, vec3 normal, vec3 viewDirection);
void Attenuate(vec3 delta, float range, inout float diffuseIntensity, inout float specularIntensity);
vec3 GetDirectionalLight(vec3 diffuseColor, vec3 specularColor, DirectionalLight directional, vec3 normal, vec3 viewDirection);
vec3 GetPointLight(vec3 diffuseColor, vec3 specularColor, PointLight point, vec3 normal, vec3 viewDirection);
vec3 GetSpotLight(vec3 diffuseColor, vec3 specularColor, SpotLight spot, vec3 normal, vec3 viewDirection, float minimumIntensity, float maximumIntensity);
float GetShadows(vec4 lightSpacePosition);

void main() 
{
	// finalColor = vec4(vec3(sqrt(1-texture(shadowMap, textureCoordinate))), 1);	
	// return;
	
	vec3 diffuseColor = material.diffuseColor;
	float alpha = 1;
	
	if (material.useDiffuseMap)
	{
		vec4 diffuseTexture = texture2D(material.diffuseMap, textureCoordinate);
		diffuseColor = diffuseTexture.rgb;
		alpha = diffuseTexture.a;
	}
	
	vec3 specularColor = material.specularColor;
	
	if (material.useSpecularMap)
		specularColor = texture2D(material.specularMap, textureCoordinate).rgb;
	
    // vec3 normal = worldNormal;
    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(worldPosition - cameraPosition);

	vec3 result = vec3(0);
	// result += light.ambientTerm * diffuseColor;
	
    //Directional light(s) - usually one
	
    for(int i = 0; i < light.directionalLightsAmount; i++) 
		result += GetDirectionalLight(diffuseColor, specularColor, directionalLights[i], normal, viewDirection);
	
    //Point lights
    for(int j = 0; j < light.pointLightsAmount; j++) 
		result += GetPointLight(diffuseColor, specularColor, pointLights[j], normal, viewDirection);   
	
    //Spot lights
    for(int k = 0; k < light.spotLightsAmount; k++) 
		result += GetSpotLight(diffuseColor, specularColor, spotLights[k], normal, viewDirection, light.ambientStrength, 1); 
	
    //Calculate shadow
    // float shadow = GetShadows(lightSpacePosition);       
    // result *= (1.0f - shadow);
	
	result += light.ambientTerm * diffuseColor;

	if (material.useEmission)
	{
		vec3 emissionColor = material.emissionColor;
		
		if (material.useEmissionMap)
			emissionColor = texture2D(material.emissionMap, textureCoordinate).rgb;
			
		result += emissionColor;
	}
	
	// finalColor = vec4(1) * (1-shadow);
	// finalColor = lightSpacePosition;  
    // float depthValue = texture(shadowMap, textureCoordinate).r;
    // finalColor = vec4(vec3(depthValue), 1.0);
	// finalColor = texture2D(shadowMap, textureCoordinate);
     finalColor = vec4(result, alpha);
	// finalColor = vec4(diffuseColor, alpha);
}

float GetDiffuseIntensity(vec3 direction, vec3 normal)
{
	return max (0, dot(direction, normal));
}

float GetSpecularIntensity(vec3 direction, vec3 normal, vec3 viewDirection)
{
	//Phong
	// return pow(max(dot(reflect(direction, normal), -viewDirection), 0), material.shininess);
	
	//Blinn-Phong
	// vec3 halfwayVector = (-direction + -viewDirection)/(length(direction) + length(viewDirection));
	vec3 halfwayVector = normalize(-direction + (-viewDirection));
	return pow(max(dot(halfwayVector, normal), 0), material.shininess);
}

void Attenuate(vec3 delta, float range, inout float diffuseIntensity, inout float specularIntensity)
{
	//Attenuation
	float linearQuadraticSum = light.attenuationConstants[1] + light.attenuationConstants[2];
	if (light.attenuationConstants[0] + linearQuadraticSum > 0.01f) 
	{
		float distanceFromLight = 0; 
		float distanceValue = 0;
		if (linearQuadraticSum > 0.01f)
		{
			distanceFromLight = length(delta);
			distanceValue = max(distanceFromLight - range / 2, 1);
		}
		
		float attenuation = 1 / 
		( light.attenuationConstants[0] 
		+ light.attenuationConstants[1] * distanceFromLight 
		+ light.attenuationConstants[2] * distanceFromLight * distanceFromLight);
		
		diffuseIntensity *= attenuation;
		specularIntensity *= attenuation;
	}
}

vec3 GetDirectionalLight(vec3 diffuseColor, vec3 specularColor,
 DirectionalLight directional, vec3 normal, vec3 viewDirection)
{
	float diffuseIntensity = GetDiffuseIntensity(-directional.direction, normal);
	float specularIntensity = GetSpecularIntensity(directional.direction, normal, viewDirection);
	
    vec3 diffuseTerm = diffuseIntensity * directional.color * diffuseColor;
	vec3 specularTerm = specularIntensity * directional.color * specularColor;

	return (diffuseTerm + specularTerm) * directional.intensity;
}

vec3 GetPointLight(vec3 diffuseColor, vec3 specularColor,
	PointLight point, vec3 normal, vec3 viewDirection)
{
	vec3 deltaDirection = point.position - worldPosition;
	// float diffuseIntensity = max (0, dot(normalize(deltaDirection), normalize (normal)));
	// float specularIntensity = pow(max(dot(reflect(-normalize(deltaDirection), normal), -viewDirection), 0), material.shininess);
	float diffuseIntensity = GetDiffuseIntensity(normalize(deltaDirection), normal);
	float specularIntensity = GetSpecularIntensity(normalize(deltaDirection), normal, viewDirection);
	
	Attenuate(deltaDirection, point.range, diffuseIntensity, specularIntensity);
	
    vec3 diffuseTerm = diffuseIntensity * point.color * diffuseColor;
	vec3 specularTerm = specularIntensity * point.color * specularColor;
	
	return (diffuseTerm + specularTerm) * point.intensity;
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
		
	// float specularIntensity = pow(max(dot(reflect(-normalize(deltaDirection), normal), -viewDirection), 0), material.shininess);
	float specularIntensity = GetSpecularIntensity(-normalize(deltaDirection), normal, viewDirection);
	
	Attenuate(deltaDirection, spot.range, diffuseIntensity, specularIntensity);
	
	specularIntensity *= diffuseIntensity;
	
    vec3 diffuseTerm = diffuseIntensity * spot.color * diffuseColor;
	vec3 specularTerm = specularIntensity * spot.color * specularColor;
	
	return (diffuseTerm + specularTerm) * spot.intensity;
}

float GetShadows(vec4 lightSpacePosition)
{
    // perform perspective divide
    vec3 projectionCoordinates = lightSpacePosition.xyz / lightSpacePosition.w;
    // transform to [0,1] range
    projectionCoordinates = projectionCoordinates * 0.5f + 0.5f;
    // get closest depth value from light's perspective (using [0,1] range lightSpacePosition as coordinate)
    float closestDepth = texture2D(shadowMap, projectionCoordinates.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projectionCoordinates.z;
    // check whether current fragment position is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0f : 0.0f;

    return shadow;
} 