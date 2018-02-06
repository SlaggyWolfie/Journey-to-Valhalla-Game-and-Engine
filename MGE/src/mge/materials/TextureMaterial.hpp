#ifndef TEXTUREMATERIAL_HPP
#define TEXTUREMATERIAL_HPP

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

/**
 * This material is already a little bit more complicated, instead of a color we can pass in a texture,
 * all attributes and uniforms are cached and we precompute the MVP matrix passing it in as one entity.
 */
class TextureMaterial : public AbstractMaterial
{
    public:
        TextureMaterial (Texture* pDiffuseTexture);
        virtual ~TextureMaterial ();

        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uDiffuseTexture;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;

        Texture* _diffuseTexture;

        TextureMaterial(const TextureMaterial&);
        TextureMaterial& operator=(const TextureMaterial&);

};

#endif // TEXTUREMATERIAL_HPP
