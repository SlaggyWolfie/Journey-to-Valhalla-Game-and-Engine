#ifndef WATER_MATERIAL_HPP
#define WATER_MATERIAL_HPP

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class WaterMaterial : public AbstractMaterial
{
public:
	WaterMaterial(Texture* pDiffuseTexture);
	virtual ~WaterMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);

protected:
private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uMVPMatrix;
	static GLint _uModelMatrix;
	static GLint _uViewMatrix;
	static GLint _uProjectionMatrix;
	static GLint _uDiffuseTexture;
	static GLint _uTime;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	//static std::chrono::time_point<std::chrono::steady_clock> _startTime;
	static sf::Clock _clock;

	Texture* _diffuseTexture;

	WaterMaterial(const WaterMaterial&);
	WaterMaterial& operator=(const WaterMaterial&);

};

#endif // WATER_MATERIAL_HPP
