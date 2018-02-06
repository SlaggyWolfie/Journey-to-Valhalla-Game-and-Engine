#include "glm.hpp"

#include "LitMaterial.hpp"
#include "myGame/MyGameConfig.hpp"
//#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"

ShaderProgram* LitMaterial::_shader = NULL;

GLint LitMaterial::_uNumberOfLights = 0;

GLint LitMaterial::_uVertexLit = 0;

GLint LitMaterial::_uModelMatrix = 0;
GLint LitMaterial::_uViewMatrix = 0;
GLint LitMaterial::_uProjectionMatrix = 0;

GLint LitMaterial::_uDiffuseColor = 0;
GLint LitMaterial::_uAmbientTerm = 0;
GLint LitMaterial::_uAttenuationConstants = 0;
GLint LitMaterial::_uSpecularColor = 0;
GLint LitMaterial::_uCameraPosition = 0;

GLint LitMaterial::_uShininess = 0;

GLint LitMaterial::_aVertex = 0;
GLint LitMaterial::_aNormal = 0;
GLint LitMaterial::_aUV = 0;

LitMaterial::LitMaterial(ShaderLit litType, glm::vec3 diffuseColor, glm::vec3 specularColor, float specularStrength, float shininess) :
	_diffuseColor(diffuseColor),
	_specularColor(specularColor),
	_specularStrength(specularStrength),
	_shininess(shininess),
	_shaderLitType(litType)
{
	//every time we create an instance of LitMaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
}

LitMaterial::LitMaterial(int litType, glm::vec3 diffuseColor, glm::vec3 specularColor, float specularStrength, float shininess) :
	_diffuseColor(diffuseColor),
	_specularColor(specularColor),
	_specularStrength(specularStrength),
	_shininess(shininess),
	_shaderLitType((ShaderLit)litType)
{
	//every time we create an instance of LitMaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
}


void LitMaterial::_lazyInitializeShader() {
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, MyGame::SHADER_PATH + "lit.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, MyGame::SHADER_PATH + "lit.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uNumberOfLights = _shader->getUniformLocation("numberOfLights");

		//light presets
		_uVertexLit = _shader->getUniformLocation("vertexLit");

		//MVP
		_uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uViewMatrix = _shader->getUniformLocation("viewMatrix");
		_uProjectionMatrix = _shader->getUniformLocation("projectionMatrix");

		//colors
		_uDiffuseColor = _shader->getUniformLocation("diffuseColor");
		_uAmbientTerm = _shader->getUniformLocation("ambientTerm");
		//vectors - can be positions or directions
		_uAttenuationConstants = _shader->getUniformLocation("attenuationConstants");
		_uSpecularColor = _shader->getUniformLocation("specularColor");
		_uCameraPosition = _shader->getUniformLocation("cameraPosition");

		//floats
		_uShininess = _shader->getUniformLocation("shininess");

		//in vectors
		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uvs");
	}
}

LitMaterial::~LitMaterial()
{
	//dtor
}

void LitMaterial::setDiffuseColor(glm::vec3 pDiffuseColor)
{
	_diffuseColor = pDiffuseColor;
}

glm::vec3 LitMaterial::getDiffuseColor()
{
	return _diffuseColor;
}
void LitMaterial::setSpecularColor(glm::vec3 specularColor)
{
	_specularColor = specularColor;
}
glm::vec3 LitMaterial::getSpecularColor()
{
	return _specularColor;
}
void LitMaterial::setShininess(float shininess)
{
	_shininess = shininess;
}
float LitMaterial::getShininess()
{
	return _shininess;
}

void LitMaterial::setShaderLitType(ShaderLit litType)
{
	_shaderLitType = litType;
}

void LitMaterial::setShaderLitType(int litType)
{
	_shaderLitType = (LitMaterial::ShaderLit)litType;
}

LitMaterial::ShaderLit LitMaterial::getShaderLitType()
{
	return _shaderLitType;
}

void LitMaterial::render(World* pWorld, Mesh* pMesh,
	const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix,
	const glm::mat4& pProjectionMatrix) {
	_shader->use();

	int lightCount = pWorld->getLightCount();

	//Pass in number of lights;
	glUniform1i(_uNumberOfLights, lightCount);

	//Pass in light information
	for (int i = 0; i < lightCount; i++)
	{
		Light* light = pWorld->getLightAt(i);
		//std::cout << "TextureMaterial has discovered light is at position:" << light->getWorldPosition() << std::endl;

		//Get uniform locations
		std::string prefix = "lights[" + std::to_string(i) + "].";

		GLint uLightType = _shader->getUniformLocation(prefix + "lightType");

		GLint uLightIntensity = _shader->getUniformLocation(prefix + "lightIntensity");

		GLint uLightColor = _shader->getUniformLocation(prefix + "lightColor");
		GLint uLightPosition = _shader->getUniformLocation(prefix + "lightPosition");
		GLint uLightDirection = _shader->getUniformLocation(prefix + "lightDirection");

		//Spotlight specific data
		GLint uFallOff = _shader->getUniformLocation(prefix + "fallOff");
		GLint uFallOffOuter = _shader->getUniformLocation(prefix + "fallOffOuter");

		//Pass in uniform information to struct via the locations calculated previously
		glUniform1i(uLightType, (int)light->getLightType());

		glUniform3fv(uLightColor, 1, glm::value_ptr(light->getColor()));
		glUniform3fv(uLightPosition, 1, glm::value_ptr(light->getLightPosition()));
		glUniform3fv(uLightDirection, 1, glm::value_ptr(light->getLightDirectionNormalized()));

		//Spotlight specific data
		glUniform1f(uFallOff, light->getFallOff());
		glUniform1f(uFallOffOuter, light->getFallOffOuter());

		glUniform1f(uLightIntensity, light->getLightIntensity());
	}

	//Set if it's vertex or fragment lit
	glUniform1i(_uVertexLit, (int)getShaderLitType());

	//Calculate and pass in the total ambient light
	glUniform3fv(_uAmbientTerm, 1, glm::value_ptr(
		Light::getAmbientContribution()
		* Light::getAmbientLightColor()
		* _diffuseColor));

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
