//LIT COLOR FRAGMENT SHADER
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

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;

uniform vec3 ambientTerm;
uniform float ambientContribution;
uniform vec3 attenuationConstants;

uniform vec3 cameraPosition;

in vec3 worldNormal;
in vec3 worldPosition;

out vec4 sColor;

vec3 GetDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	float diffuseIntensity = max (0, dot(-light.direction, normal));
	float specularIntensity = pow(max(dot(reflect(light.direction, normal), -viewDirection), 0), shininess);
	
    vec3 diffuseTerm = diffuseIntensity * light.color * diffuseColor;
	vec3 specularTerm = specularIntensity * light.color * specularColor;
	
	return /*ambientTerm + */diffuseTerm + specularTerm;
}

vec3 GetPointLight(PointLight light, vec3 normal, vec3 viewDirection)
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
	
    vec3 diffuseTerm = diffuseIntensity * light.color * diffuseColor;
	vec3 specularTerm = specularIntensity * light.color * specularColor;
	
	return /*ambientTerm + */diffuseTerm + specularTerm;
}

vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 viewDirection, float minimumIntensity, float maximumIntensity)
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
	
    vec3 diffuseTerm = diffuseIntensity * light.color * diffuseColor;
	vec3 specularTerm = specularIntensity * light.color * specularColor;
	
	return /*ambientTerm + */diffuseTerm + specularTerm;
}

void main (void) 
{
    vec3 normal = normalize(worldNormal);
    vec3 viewDirection = normalize(worldPosition - cameraPosition);

	vec3 result = ambientTerm * diffuseColor;
	
    //Directional light(s) - usually one
    for(int i = 0; i < numberOfDirectionalLights; i++) result += GetDirectionalLight(directionalLights[i], normal, viewDirection);
	
    //Point lights
    for(int j = 0; j < numberOfPointLights; j++) result += GetPointLight(pointLights[j], normal, viewDirection);   
	
    //Spot lights
    for(int k = 0; k < numberOfSpotLights; k++) result += GetSpotLight(spotLights[k], normal, viewDirection, ambientContribution, 1);   
	
    sColor = vec4(result, 1);
}
