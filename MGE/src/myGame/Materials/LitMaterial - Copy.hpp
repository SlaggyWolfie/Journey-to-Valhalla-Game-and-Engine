#ifndef LitMATERIAL_HPP
#define LitMATERIAL_HPP

#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"

class ShaderProgram;

/**
 * This is about the simplest material we can come up with, it demonstrates how to
 * render a single color material without caching, passing in all the matrices we require separately.
 */
class LitMaterial : public AbstractMaterial
{
public:
	enum ShaderLit { Vertex, Fragment };

	LitMaterial(
		ShaderLit litType = ShaderLit::Fragment, 
		glm::vec3 diffuseColor = glm::vec3(1, 0, 0), 
		glm::vec3 specularColor = glm::vec3(1),
		float specularStrength = 0.8f,
		float shininess = 1);

	LitMaterial(
		int litType = 1,
		glm::vec3 diffuseColor = glm::vec3(1, 0, 0), 
		glm::vec3 specularColor = glm::vec3(1),
		float specularStrength = 0.8f,
		float shininess = 1);
	virtual ~LitMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	//in rgb values
	void setDiffuseColor(glm::vec3 pDiffuseColor);
	glm::vec3 getDiffuseColor();
	void setSpecularColor(glm::vec3 specularColor);
	glm::vec3 getSpecularColor();
	void setShininess(float shininess);
	float getShininess();

	virtual void setShaderLitType(ShaderLit litType);
	virtual void setShaderLitType(int litType);
	virtual ShaderLit getShaderLitType();

	//void setAmbientColor(glm::vec3 ambientColor);
	//void setLightColor(glm::vec3 lightColor);
	//void setLightVector(glm::vec3 lightVector);

private:
	//all the static properties are shared between instances of LitMaterial
	//note that they are all PRIVATE, we do not expose this static info to the outside world
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//cache all the uniform and attribute indexes
	static GLint _uModelMatrix;
	static GLint _uViewMatrix;
	static GLint _uProjectionMatrix;

	static GLint _uNumberOfLights;

	static GLint _uVertexLit;

	static GLint _uDiffuseColor;
	static GLint _uAmbientTerm;
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

	ShaderLit _shaderLitType;
	//glm::vec3 _ambientLightColor;
	//glm::vec3 _lightColor;
	//glm::vec3 _lightVector;
};

#endif // LitMATERIAL_HPP
