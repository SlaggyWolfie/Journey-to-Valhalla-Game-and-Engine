#include "glm.hpp"

#include "WaterMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "myGame/MyGameConfig.hpp"

ShaderProgram* WaterMaterial::_shader = NULL;

GLint WaterMaterial::_uMVPMatrix = 0;
GLint WaterMaterial::_uModelMatrix = 0;
GLint WaterMaterial::_uViewMatrix = 0;
GLint WaterMaterial::_uProjectionMatrix = 0;
GLint WaterMaterial::_uDiffuseTexture = 0;
GLint WaterMaterial::_uTime = 0;

GLint WaterMaterial::_aVertex = 0;
GLint WaterMaterial::_aNormal = 0;
GLint WaterMaterial::_aUV = 0;

sf::Clock WaterMaterial::_clock = sf::Clock();

WaterMaterial::WaterMaterial(Texture * pDiffuseTexture) :_diffuseTexture(pDiffuseTexture) {
	_lazyInitializeShader();
}

WaterMaterial::~WaterMaterial() {}

void WaterMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, MyGame::SHADER_PATH + "water.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, MyGame::SHADER_PATH + "water.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uViewMatrix = _shader->getUniformLocation("viewMatrix");
		_uProjectionMatrix = _shader->getUniformLocation("projectionMatrix");
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");
		_uTime = _shader->getUniformLocation("time");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");

		//_startTime = std::chrono::high_resolution_clock::now();
		_clock = sf::Clock();
	}
}

void WaterMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void WaterMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "WaterMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture, 0);

	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_uViewMatrix, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));

	//std::cout << (GLint)(std::chrono::system_clock::now().time_since_epoch().count()) <<std::endl;
	glUniform1f(_uTime, _clock.getElapsedTime().asSeconds());

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
