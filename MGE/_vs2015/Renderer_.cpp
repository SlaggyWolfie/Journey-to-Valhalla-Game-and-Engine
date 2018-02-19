#include "Renderer_.hpp"
#include "GameObject_.hpp"
#include "Mesh_.hpp"
#include "Material_.hpp"
namespace Engine
{
	namespace Rendering
	{
		glm::uint Renderer_::_uModelMatrix = 0;
		glm::uint Renderer_::_uViewMatrix = 0;
		glm::uint Renderer_::_uProjectionMatrix = 0;

		glm::uint Renderer_::_uMVP_Matrix = 0;
		glm::uint Renderer_::_uMV_Matrix = 0;
		glm::uint Renderer_::_uVP_Matrix = 0;

		glm::uint Renderer_::_uNormalMatrix = 0;

		glm::uint Renderer_::_uDiffuseColor = 0;
		glm::uint Renderer_::_uSpecularColor = 0;
		glm::uint Renderer_::_uEmissionColor = 0;

		glm::uint Renderer_::_uDiffuseMap = 0;
		glm::uint Renderer_::_uSpecularMap = 0;
		glm::uint Renderer_::_uEmissionMap = 0;

		glm::uint Renderer_::_aPositions = 0;
		glm::uint Renderer_::_aNormals = 0;
		glm::uint Renderer_::_aUVs = 0;
		glm::uint Renderer_::_aIndices = 0;

		Renderer_::Renderer_(): 
		_material(nullptr), _mesh(nullptr), _renderQueue(Opaque), _castsShadows(true)
		{
		}

		Renderer_::Renderer_(Material_* material, Mesh_* mesh, const RenderQueue renderQueue, const bool castShadows): 
		_material(material), _mesh(mesh), _renderQueue(renderQueue), _castsShadows(castShadows)
		{
		}

		bool Renderer_::shouldCastShadows(const bool castShadows)
		{
			_castsShadows = castShadows;
			return castShadows;
		}

		bool Renderer_::castsShadows() const
		{
			return _castsShadows;
		}

		void Renderer_::destroy()
		{
			Component::destroy();
			delete this;
		}

		void Renderer_::setupGL()
		{
		}

		void Renderer_::render()
		{
		}

		void Renderer_::findMesh()
		{
			_mesh = getGameObject()->getComponent<Mesh_>();
		}

		void Renderer_::findMaterial()
		{
			_material = getGameObject()->getComponent<Material_>();
		}

		bool Renderer_::isUniquePerGameObject()
		{
			return true;
		}

		void Renderer_::prewake()
		{
			if (_mesh == nullptr || _material == nullptr)
			{
				findMesh();
				findMaterial();
			}
		}
	}
}