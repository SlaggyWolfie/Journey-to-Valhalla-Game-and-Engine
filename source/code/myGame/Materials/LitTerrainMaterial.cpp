#include "glm.hpp"

#include "LitTerrainMaterial.hpp"
#include "myGame/MyGameConfig.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"

ShaderProgram* LitTerrainMaterial::_shader = NULL;

GLint LitTerrainMaterial::_uDiffuseTexture0 = 0;
GLint LitTerrainMaterial::_uDiffuseTexture1 = 0;
GLint LitTerrainMaterial::_uDiffuseTexture2 = 0;
GLint LitTerrainMaterial::_uDiffuseTexture3 = 0;

GLint LitTerrainMaterial::_uHeightmap = 0;
GLint LitTerrainMaterial::_uSplatmap = 0;

GLint LitTerrainMaterial::_uTerrainHeight = 0;
GLint LitTerrainMaterial::_uTerrainWidth = 0;
GLint LitTerrainMaterial::_uTerrainLength = 0;

GLint LitTerrainMaterial::_uNumberOfDirectionalLights = 0;
GLint LitTerrainMaterial::_uNumberOfPointLights = 0;
GLint LitTerrainMaterial::_uNumberOfSpotLights = 0;

//GLint LitTerrainMaterial::_uVertexLit = 0;

GLint LitTerrainMaterial::_uModelMatrix = 0;
GLint LitTerrainMaterial::_uViewMatrix = 0;
GLint LitTerrainMaterial::_uProjectionMatrix = 0;

GLint LitTerrainMaterial::_uDiffuseColor = 0;
GLint LitTerrainMaterial::_uAmbientTerm = 0;
GLint LitTerrainMaterial::_uAmbientContribution = 0;
GLint LitTerrainMaterial::_uAttenuationConstants = 0;
GLint LitTerrainMaterial::_uSpecularColor = 0;
GLint LitTerrainMaterial::_uCameraPosition = 0;

GLint LitTerrainMaterial::_uShininess = 0;

GLint LitTerrainMaterial::_aVertex = 0;
GLint LitTerrainMaterial::_aNormal = 0;
GLint LitTerrainMaterial::_aUV = 0;

LitTerrainMaterial::LitTerrainMaterial(
	Texture * heightmap, Texture * splatmap, 
	Texture * texture1, Texture * texture2, Texture * texture3, Texture * texture4, float terrainWidth, float terrainLength, float terrainHeight,
	glm::vec3 diffuseColor, glm::vec3 specularColor, float specularStrength, float shininess) :
	_diffuseColor(diffuseColor),
	_specularColor(specularColor),
	_specularStrength(specularStrength),
	_shininess(shininess),
	_terrainWidth(terrainWidth),
	_terrainLength(terrainLength),
	_terrainHeight(terrainHeight),
	_heightmap(heightmap), _splatmap(splatmap),
	_diffuseTexture0(texture1), _diffuseTexture1(texture2), _diffuseTexture2(texture3), _diffuseTexture3(texture4)
{
	_lazyInitializeShader();
}


void LitTerrainMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, MyGame::SHADER_PATH + "litTerrain.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, MyGame::SHADER_PATH + "litTerrain.fs");

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
		_uDiffuseTexture0 = _shader->getUniformLocation("diffuseTexture0");
		_uDiffuseTexture1 = _shader->getUniformLocation("diffuseTexture1");
		_uDiffuseTexture2 = _shader->getUniformLocation("diffuseTexture2");
		_uDiffuseTexture3 = _shader->getUniformLocation("diffuseTexture3");

		_uHeightmap = _shader->getUniformLocation("heightmap");
		_uSplatmap = _shader->getUniformLocation("splatmap");

		_uTerrainHeight = _shader->getUniformLocation("terrainHeight");
		_uTerrainWidth = _shader->getUniformLocation("terrainWidth");
		_uTerrainLength = _shader->getUniformLocation("terrainLength");

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

LitTerrainMaterial::~LitTerrainMaterial()
{
	//dtor
}

void LitTerrainMaterial::setDiffuseColor(glm::vec3 pDiffuseColor)
{
	_diffuseColor = pDiffuseColor;
}

glm::vec3 LitTerrainMaterial::getDiffuseColor()
{
	return _diffuseColor;
}

//void LitTerrainMaterial::setDiffuseTexture(Texture * texture)
//{
//	_diffuseTexture = texture;
//}
//Texture * LitTerrainMaterial::getDiffuseTexure()
//{
//	return _diffuseTexture;
//}

void LitTerrainMaterial::setSpecularColor(glm::vec3 specularColor)
{
	_specularColor = specularColor;
}
glm::vec3 LitTerrainMaterial::getSpecularColor()
{
	return _specularColor;
}
void LitTerrainMaterial::setSpecularStrength(float specularStrength)
{
	_specularStrength = specularStrength;
}
float LitTerrainMaterial::getSpecularStrength()
{
	return _specularStrength;
}
void LitTerrainMaterial::setShininess(float shininess)
{
	_shininess = shininess;
}
float LitTerrainMaterial::getShininess()
{
	return _shininess;
}

void LitTerrainMaterial::setShaderLitType(ShaderLit litType)
{
	_shaderLitType = litType;
}

void LitTerrainMaterial::setShaderLitType(int litType)
{
	_shaderLitType = (LitTerrainMaterial::ShaderLit)litType;
}

LitTerrainMaterial::ShaderLit LitTerrainMaterial::getShaderLitType()
{
	return _shaderLitType;
}

void LitTerrainMaterial::render(World* pWorld, Mesh* pMesh,
	const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix,
	const glm::mat4& pProjectionMatrix) {

	if (!_diffuseTexture0 ||
		!_diffuseTexture1 || 
		!_diffuseTexture2 || 
		!_diffuseTexture3 ||
		!_heightmap || !_splatmap) return;

	_shader->use();

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture0->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture0, 0);

	//setup texture slot 1
	glActiveTexture(GL_TEXTURE1);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture1->getId());
	//tell the shader the texture slot for the diffuse texture is slot 1
	glUniform1i(_uDiffuseTexture1, 1);

	//setup texture slot 2
	glActiveTexture(GL_TEXTURE2);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture2->getId());
	//tell the shader the texture slot for the diffuse texture is slot 2
	glUniform1i(_uDiffuseTexture2, 2);

	//setup texture slot 3
	glActiveTexture(GL_TEXTURE3);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture3->getId());
	//tell the shader the texture slot for the diffuse texture is slot 3
	glUniform1i(_uDiffuseTexture3, 3);

	//setup texture slot 4
	glActiveTexture(GL_TEXTURE4);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _heightmap->getId());
	//tell the shader the texture slot for the diffuse texture is slot 3
	glUniform1i(_uHeightmap, 4);

	//setup texture slot 5
	glActiveTexture(GL_TEXTURE5);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _splatmap->getId());
	//tell the shader the texture slot for the diffuse texture is slot 3
	glUniform1i(_uSplatmap, 5);

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

	glUniform1f(_uTerrainHeight, _terrainHeight * _owner->getScale().y * 2);
	glUniform1f(_uTerrainLength, _terrainLength * _owner->getScale().z * 2);
	glUniform1f(_uTerrainWidth, _terrainWidth * _owner->getScale().x * 2);
	
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
