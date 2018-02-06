#ifndef LIT_TERRAIN_MATERIAL_HPP
#define LIT_TERRAIN_MATERIAL_HPP

#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"

class ShaderProgram;
class Texture;

/**
 * This is about the simplest material we can come up with, it demonstrates how to
 * render a single color material without caching, passing in all the matrices we require separately.
 */
class LitTerrainMaterial : public AbstractMaterial
{
public:
	enum ShaderLit { Vertex, Fragment };

	LitTerrainMaterial(
		Texture* heightmap,
		Texture* splatmap,
		Texture* texture1,
		Texture* texture2,
		Texture* texture3,
		Texture* texture4,
		float terrainWidth = 1,
		float terrainLength = 1,
		float terrainHeight = 1,
		glm::vec3 diffuseColor = glm::vec3(1, 0, 0), 
		glm::vec3 specularColor = glm::vec3(1),
		float specularStrength = 0.1f,
		float shininess = 0.1f);
	virtual ~LitTerrainMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	//in rgb values
	void setDiffuseColor(glm::vec3 pDiffuseColor);
	glm::vec3 getDiffuseColor();

	//void setDiffuseTexture(Texture* texture);
	//Texture* getDiffuseTexure();

	void setSpecularColor(glm::vec3 specularColor);
	glm::vec3 getSpecularColor();

	virtual void setSpecularStrength(float specularStrength);
	virtual float getSpecularStrength();

	void setShininess(float shininess);
	float getShininess();

	virtual void setShaderLitType(ShaderLit litType);
	virtual void setShaderLitType(int litType);
	virtual ShaderLit getShaderLitType();

private:
	//all the static properties are shared between instances of LitTerrainMaterial
	//note that they are all PRIVATE, we do not expose this static info to the outside world
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//cache all the uniform and attribute indexes
	static GLint _uModelMatrix;
	static GLint _uViewMatrix;
	static GLint _uProjectionMatrix;

	static GLint _uNumberOfDirectionalLights;
	static GLint _uNumberOfPointLights;
	static GLint _uNumberOfSpotLights;

	//static GLint _uVertexLit;

	static GLint _uDiffuseTexture0;
	static GLint _uDiffuseTexture1;
	static GLint _uDiffuseTexture2;
	static GLint _uDiffuseTexture3;
	static GLint _uHeightmap;
	static GLint _uSplatmap;

	static GLint _uTerrainHeight;
	static GLint _uTerrainWidth;
	static GLint _uTerrainLength;

	static GLint _uDiffuseColor;
	static GLint _uAmbientTerm;
	static GLint _uAmbientContribution;
	static GLint _uAttenuationConstants;
	static GLint _uSpecularColor;
	static GLint _uCameraPosition;

	static GLint _uShininess;

	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	//this one is unique per instance of color material
	glm::vec3 _diffuseColor;
	glm::vec3 _specularColor;
	float _shininess;
	float _specularStrength;

	//Terrain stuff
	float _terrainHeight;
	float _terrainWidth;
	float _terrainLength;

	ShaderLit _shaderLitType;

	Mesh* _mesh;

	Texture* _heightmap;
	Texture* _splatmap;
	Texture* _diffuseTexture0;
	Texture* _diffuseTexture1;
	Texture* _diffuseTexture2;
	Texture* _diffuseTexture3;
};

#endif // LIT_TERRAIN_MATERIAL_HPP
