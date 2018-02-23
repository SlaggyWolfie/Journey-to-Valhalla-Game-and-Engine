#include "glm.hpp"

#include "LitTextureMaterial.hpp"
#include "myGame/MyGameConfig.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"

ShaderProgram* LitTextureMaterial::_shader = NULL;

GLint LitTextureMaterial::_uDiffuseTexture = 0;

GLint LitTextureMaterial::_uNumberOfDirectionalLights = 0;
GLint LitTextureMaterial::_uNumberOfPointLights = 0;
GLint LitTextureMaterial::_uNumberOfSpotLights = 0;

//GLint LitTextureMaterial::_uVertexLit = 0;

GLint LitTextureMaterial::_uModelMatrix = 0;
GLint LitTextureMaterial::_uViewMatrix = 0;
GLint LitTextureMaterial::_uProjectionMatrix = 0;

GLint LitTextureMaterial::_uDiffuseColor = 0;
GLint LitTextureMaterial::_uAmbientTerm = 0;
GLint LitTextureMaterial::_uAmbientContribution = 0;
GLint LitTextureMaterial::_uAttenuationConstants = 0;
GLint LitTextureMaterial::_uSpecularColor = 0;
GLint LitTextureMaterial::_uCameraPosition = 0;

GLint LitTextureMaterial::_uShininess = 0;

GLint LitTextureMaterial::_aVertex = 0;
GLint LitTextureMaterial::_aNormal = 0;
GLint LitTextureMaterial::_aUV = 0;

//LitTextureMaterial::LitTextureMaterial(ShaderLit litType, glm::vec3 diffuseColor, glm::vec3 specularColor, float specularStrength, float shininess) :
//	_diffuseColor(diffuseColor),
//	_specularColor(specularColor),
//	_specularStrength(specularStrength),
//	_shininess(shininess),
//	_shaderLitType(litType)
//{
//	//every time we create an instance of LitTextureMaterial we check if the corresponding shader has already been loaded
//	_lazyInitializeShader();
//}

//LitTextureMaterial::LitTextureMaterial(int litType, glm::vec3 diffuseColor, glm::vec3 specularColor, float specularStrength, float shininess) :
//	_diffuseColor(diffuseColor),
//	_specularColor(specularColor),
//	_specularStrength(specularStrength),
//	_shininess(shininess),
//	_shaderLitType((ShaderLit)litType)

LitTextureMaterial::LitTextureMaterial(Texture* texture, glm::vec3 diffuseColor, glm::vec3 specularColor,
	float specularStrength, float shininess) :
	_diffuseColor(diffuseColor),
	_specularColor(specularColor),
	_specularStrength(specularStrength),
	_shininess(shininess),
	_diffuseTexture(texture)
{
	//every time we create an instance of LitTextureMaterial we check if the corresponding shader has already been loaded

	//std::cout << _diffuseTexture << " " << texture << std::endl;
	_lazyInitializeShader();
}


void LitTextureMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, MyGame::SHADER_PATH + "litTexture.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, MyGame::SHADER_PATH + "litTexture.fs");
		//_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "texture.vs");
		//_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "texture.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uNumberOfDirectionalLights = _shader->getUniformLocation("numberOfDirectionalLights");
		_uNumberOfPointLights = _shader->getUniformLocation("numberOfPointLights");
		_uNumberOfSpotLights = _shader->getUniformLocation("numberOfSpotLights");

		//light presets
		//_uVertexLit = _shader->getUniformLocation("vertexLit");

		//MVP
		_uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uViewMatrix = _shader->getUniformLocation("viewMatrix");
		_uProjectionMatrix = _shader->getUniformLocation("projectionMatrix");

		//texture
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");
		std::cout << _uDiffuseTexture << std::endl;

		//colors
		_uDiffuseColor = _shader->getUniformLocation("diffuseColor");
		_uAmbientTerm = _shader->getUniformLocation("ambientTerm");
		//vectors - can be positions or directions
		_uAttenuationConstants = _shader->getUniformLocation("attenuationConstants");
		_uSpecularColor = _shader->getUniformLocation("specularColor");
		_uCameraPosition = _shader->getUniformLocation("cameraPosition");

		//floats
		_uShininess = _shader->getUniformLocation("shininess");
		_uAmbientContribution = _shader->getUniformLocation("ambientContribution");

		//in vectors
		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

LitTextureMaterial::~LitTextureMaterial()
{
	//dtor
}

void LitTextureMaterial::setDiffuseColor(glm::vec3 pDiffuseColor)
{
	_diffuseColor = pDiffuseColor;
}

glm::vec3 LitTextureMaterial::getDiffuseColor()
{
	return _diffuseColor;
}
void LitTextureMaterial::setDiffuseTexture(Texture * texture)
{
	_diffuseTexture = texture;
}
Texture * LitTextureMaterial::getDiffuseTexure()
{
	return _diffuseTexture;
}
void LitTextureMaterial::setSpecularColor(glm::vec3 specularColor)
{
	_specularColor = specularColor;
}
glm::vec3 LitTextureMaterial::getSpecularColor()
{
	return _specularColor;
}
void LitTextureMaterial::setSpecularStrength(float specularStrength)
{
	_specularStrength = specularStrength;
}
float LitTextureMaterial::getSpecularStrength()
{
	return _specularStrength;
}
void LitTextureMaterial::setShininess(float shininess)
{
	_shininess = shininess;
}
float LitTextureMaterial::getShininess()
{
	return _shininess;
}

void LitTextureMaterial::setShaderLitType(ShaderLit litType)
{
	_shaderLitType = litType;
}

void LitTextureMaterial::setShaderLitType(int litType)
{
	_shaderLitType = (LitTextureMaterial::ShaderLit)litType;
}

LitTextureMaterial::ShaderLit LitTextureMaterial::getShaderLitType()
{
	return _shaderLitType;
}

void LitTextureMaterial::render(World* pWorld, Mesh* pMesh,
	const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix,
	const glm::mat4& pProjectionMatrix) {

	if (!_diffuseTexture) return;

	_shader->use();

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture, 0);


	int lightCount = pWorld->getLightCount();
	int directionalLightCount = 0;
	int pointLightCount = 0;
	int spotLightCount = 0;

	//Pass in light information
	for (int i = 0; i < lightCount; i++)
	{
		Light* light = pWorld->getLightAt(i);
		//std::cout << "TextureMaterial has discovered light is at position:" << light->getWorldPosition() << std::endl;

		std::string prefix = "lights[";

		//Count the number of lights
		switch (light->getLightType())
		{
		case Light::Type::Directional:
		{
			prefix = "directionalLights[" + std::to_string(directionalLightCount) + "].";
			directionalLightCount++;

			//Get uniform locations
			GLint uLightColor = _shader->getUniformLocation(prefix + "color");
			GLint uLightDirection = _shader->getUniformLocation(prefix + "direction");

			GLint uLightIntensity = _shader->getUniformLocation(prefix + "intensity");

			//Pass in uniform information to struct via the locations calculated previously
			glUniform3fv(uLightColor, 1, glm::value_ptr(light->getColor()));
			glUniform3fv(uLightDirection, 1, glm::value_ptr(light->getLightDirectionNormalized()));

			glUniform1f(uLightIntensity, light->getLightIntensity());
			break;
		}
		case Light::Type::Point:
		{
			prefix = "pointLights[" + std::to_string(pointLightCount) + "].";
			pointLightCount++;

			//Get uniform locations
			GLint uLightColor = _shader->getUniformLocation(prefix + "color");
			GLint uLightPosition = _shader->getUniformLocation(prefix + "position");

			GLint uLightIntensity = _shader->getUniformLocation(prefix + "intensity");

			//Pass in uniform information to struct via the locations calculated previously
			glUniform3fv(uLightColor, 1, glm::value_ptr(light->getColor()));
			glUniform3fv(uLightPosition, 1, glm::value_ptr(light->getLightPosition()));

			glUniform1f(uLightIntensity, light->getLightIntensity());
			break;
		}
		case Light::Type::Spotlight:
		{
			prefix = "spotLights[" + std::to_string(spotLightCount) + "].";
			spotLightCount++;

			//Get uniform locations
			GLint uLightColor = _shader->getUniformLocation(prefix + "color");
			GLint uLightPosition = _shader->getUniformLocation(prefix + "position");
			GLint uLightDirection = _shader->getUniformLocation(prefix + "direction");

			//Spotlight specific data
			GLint uFallOff = _shader->getUniformLocation(prefix + "fallOff");
			GLint uFallOffOuter = _shader->getUniformLocation(prefix + "fallOffOuter");

			//GLint uLightIntensity = _shader->getUniformLocation(prefix + "intensity");

			//Pass in uniform information to struct via the locations calculated previously
			glUniform3fv(uLightColor, 1, glm::value_ptr(light->getColor() * light->getLightIntensity()));
			glUniform3fv(uLightPosition, 1, glm::value_ptr(light->getLightPosition()));
			glUniform3fv(uLightDirection, 1, glm::value_ptr(light->getLightDirectionNormalized()));

			//Spotlight specific data
			glUniform1f(uFallOff, light->getFallOff());
			glUniform1f(uFallOffOuter, light->getFallOffOuter());

			//glUniform1f(uLightIntensity, 1);
			//glUniform1f(uLightIntensity, light->getLightIntensity());
			break;
		}
		default:
		{
			std::cout << "Unindentified light type: " + std::to_string(light->getLightType());
			return;
		}
		}
	}

	glUniform1i(_uNumberOfDirectionalLights, directionalLightCount);
	glUniform1i(_uNumberOfPointLights, pointLightCount);
	glUniform1i(_uNumberOfSpotLights, spotLightCount);

	//std::cout << "Number of Directional Lights detected: " + std::to_string(directionalLightCount) << std::endl;
	//std::cout << "Number of Point Lights detected: " + std::to_string(pointLightCount) << std::endl;
	//std::cout << "Number of Spot Lights detected: " + std::to_string(spotLightCount) << std::endl;

	//Set if it's vertex or fragment lit
	//glUniform1i(_uVertexLit, (int)getShaderLitType());

	//Calculate and pass in the total ambient light
	glUniform3fv(_uAmbientTerm, 1, glm::value_ptr(
		Light::getAmbientContribution()
		* Light::getAmbientLightColor()));
	glUniform1f(_uAmbientContribution, Light::getAmbientContribution());

	//Pass in the light attenuation parameters
	glUniform3fv(_uAttenuationConstants, 1, glm::value_ptr(Light::getAttenuationConstants()));

	//Calculate total specular color and pass it in and the material shininess
	glUniform3fv(_uSpecularColor, 1, glm::value_ptr(_specularStrength * _specularColor));
	glUniform1f(_uShininess, _shininess);

	//Create a fallback camera position (or get the actual one if possible) and pass it in
	glm::vec3 cameraPosition = glm::vec3(0);
	Camera* mainCamera = pWorld->getMainCamera();
	if (mainCamera != NULL) cameraPosition = mainCamera->getWorldPosition();
	glUniform3fv(_uCameraPosition, 1, glm::value_ptr(cameraPosition));

	//set the material color
	glUniform3fv(_uDiffuseColor, 1, glm::value_ptr(_diffuseColor));

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_uViewMatrix, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
