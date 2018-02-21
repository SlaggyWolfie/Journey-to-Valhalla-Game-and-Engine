#include "Renderer_.hpp"
#include "GameObject_.hpp"
#include "Mesh_.hpp"
#include "Material_.hpp"
namespace Engine
{
	namespace Rendering
	{
		glm::uint Renderer_::_uniformModelMatrix = 0;
		glm::uint Renderer_::_uniformViewMatrix = 0;
		glm::uint Renderer_::_uniformProjectionMatrix = 0;

		glm::uint Renderer_::_uniformMVP_Matrix = 0;
		glm::uint Renderer_::_uniformMV_Matrix = 0;
		glm::uint Renderer_::_uniformVP_Matrix = 0;

		glm::uint Renderer_::_uniformNormalMatrix = 0;

		glm::uint Renderer_::_uniformDiffuseColor = 0;
		glm::uint Renderer_::_uniformSpecularColor = 0;
		glm::uint Renderer_::_uniformEmissionColor = 0;

		glm::uint Renderer_::_uniformDiffuseMap = 0;
		glm::uint Renderer_::_uniformSpecularMap = 0;
		glm::uint Renderer_::_uniformEmissionMap = 0;

		glm::uint Renderer_::_attributePositions = 0;
		glm::uint Renderer_::_attributeNormals = 0;
		glm::uint Renderer_::_attributeUVs = 0;
		glm::uint Renderer_::_attributeIndices = 0;

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
			return;
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