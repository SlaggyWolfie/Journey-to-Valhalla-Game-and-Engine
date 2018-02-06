//LIT COLOR FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

// struct Light
// {
	// int lightType;

	// vec3 lightColor;
	// vec3 lightDirection;
	// vec3 lightPosition;
	// vec3 attenuationConstants;

	// float fallOff;
	// float fallOffOuter;
	
	// float lightIntensity;
// }

uniform int vertexLit;
uniform int lightType;

uniform vec3 diffuseColor;
uniform vec3 ambientLightColor;
uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 ambientTerm;
uniform vec3 attenuationConstants;
uniform vec3 specularColor;
uniform vec3 cameraPosition;

uniform float fallOff;
uniform float fallOffOuter;
// uniform float ambientContribution;
uniform float lightIntensity;
uniform float shininess;

in float diffuseIntensityFromVertex;
in vec3 worldNormal;
in vec3 worldPosition;

out vec4 sColor;

// void GetDiffuseIntensity(out float diffuseIntensity, inout vec3 delta)
// {
	// diffuseIntensity = diffuseIntensityFromVertex;
	
	// if (vertexLit != 0)
	// {
		// if (lightType == 0) delta = -lightDirection;
		// else if (lightType == 1) diffuseIntensity = max (0, dot(normalize(delta), normalize (worldNormal)));
		// else if (lightType == 2) 
		// {
			// float angle = dot(-normalize(delta), -lightDirection);
			// if (angle > fallOff) diffuseIntensity = 0.8f;
		// }
	// }
// } 

void GetDiffuseIntensity(out float diffuseIntensity, out vec3 delta)
{
	diffuseIntensity = diffuseIntensityFromVertex;
	
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
				float incidentAngle = dot(normalize(delta), normalize(lightDirection));
				// if (incidentAngle > fallOffOuter) diffuseIntensity = 0.8f;
				float fallOffDelta = fallOff - fallOffOuter;
				diffuseIntensity = clamp((incidentAngle - fallOffOuter) / fallOffDelta, 0.0f, 0.8f);   
				break;
			}
			default:
			{
				discard;
				break;
			}
		};
	}
} 

void GetSpecularIntensity(out float specularIntensity, const in vec3 delta)
{
	specularIntensity = 
	pow(
		max(
			dot(
				reflect(-normalize(delta), normalize (worldNormal))
				, 
				normalize(cameraPosition - worldPosition)
			), 
		0), 
	shininess);
}

void Attenuate(inout float diffuseIntensity, inout float specularIntensity, const in vec3 delta)
{
	switch(lightType)
	{
		case 0: break;
		case 1:
		{
			float linearQuadraticSum = attenuationConstants[1] + attenuationConstants[2];
			if (attenuationConstants[0] + linearQuadraticSum > 0.01f) 
			{
				float distanceFromLight = 0; 
				if (linearQuadraticSum > 0.01f) distanceFromLight = length(delta);
				
				float attenuation = 1 / 
				( attenuationConstants[0] 
				+ attenuationConstants[1] * distanceFromLight 
				+ attenuationConstants[2] * distanceFromLight * distanceFromLight);
				
				//Attenuation
				
				//cannot edit uniforms - still possible to get the results if I wanted
				//ambientTerm *= attenuation;
				diffuseIntensity *= attenuation;
				specularIntensity *= attenuation;
			}
			
			break;
		}
		case 2:
		{
			specularIntensity *= diffuseIntensity;
			break;
		}
		default:
		{
			discard;
			break;
		}
	}
}

void main (void) {
	
	vec3 delta = lightPosition - worldPosition;
	
	float diffuseIntensity = 0;
	float specularIntensity = 0;
	
	GetDiffuseIntensity(diffuseIntensity, delta);
	GetSpecularIntensity(specularIntensity, delta);
	
	Attenuate(diffuseIntensity, specularIntensity, delta);
	
	//Ambient Term now passed in (calculated on CPU once per shader)
    //vec3 ambientTerm_ = ambientContribution * ambientLightColor * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * (lightIntensity * lightColor) * diffuseColor;
	vec3 specularTerm = specularIntensity * lightColor * specularColor;
	
	// specularTerm = vec3(0,0,0);

    sColor = vec4 (ambientTerm + diffuseTerm + specularTerm , 1);
}
