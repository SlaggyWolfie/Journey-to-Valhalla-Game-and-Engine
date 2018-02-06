//LIT COLOR FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;
//uniform vec3 cameraPosition;

//in vec3 worldNormal;
//in vec3 worldPosition;
in vec2 texCoord;

out vec4 sColor;
void main (void) 
{

	// vec4 diffuseTextureColorA = texture2D(diffuseTexture, vec2( 0, 0 ));
	//vec4 diffuseTextureColorA = texture(diffuseTexture, texCoord);
	
	//vec3 diffuseTextureColor = diffuseTextureColorA.rgb;
	
    ///vec3 normal = normalize(worldNormal);
   // vec3 viewDirection = normalize(worldPosition - cameraPosition);

	//vec3 result = vec3(0,0,0);
	
    //Directional light(s) - usually one
    // for(int i = 0; i < numberOfDirectionalLights; i++) result += GetDirectionalLight(diffuseTextureColor, directionalLights[i], normal, viewDirection);
	
    //Point lights
    // for(int j = 0; j < numberOfPointLights; j++) result += GetPointLight(diffuseTextureColor, pointLights[j], normal, viewDirection);   
	
    //Spot lights
    // for(int k = 0; k < numberOfSpotLights; k++) result += GetSpotLight(diffuseTextureColor, spotLights[k], normal, viewDirection, ambientContribution, 1);   
	
    // sColor = vec4(result, diffuseTextureColorA.a);
	//sColor = diffuseTextureColorA;
	sColor = texture(diffuseTexture, texCoord);
	//sColor = vec4(1,1,1,1);
}
