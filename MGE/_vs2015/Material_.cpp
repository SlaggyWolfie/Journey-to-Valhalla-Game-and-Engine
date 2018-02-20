#include "Material_.hpp"

namespace Engine
{
	namespace Rendering
	{

		Material_::Material_() :
			_diffuseColor(glm::vec3(1)), _specularColor(glm::vec3(1)), _emissionColor(glm::vec3(0, 1, 0)),
			_diffuseMap(nullptr), _specularMap(nullptr), _emissionMap(nullptr),
			_useDiffuseMap(false), _useSpecularMap(false), _useEmissionMap(false), _useEmission(false)
		{
		}

		Material_::~Material_()
		{
			_diffuseMap = nullptr;
			_specularMap = nullptr;
			_emissionMap = nullptr;
		}

		Shader* Material_::getShader()
		{
			//return _shader;
			return nullptr;
		}

		glm::vec3 Material_::getSpecularColor() const
		{
			return _specularColor;
		}

		void Material_::setEmissionColor(const glm::vec3 color)
		{
			_emissionColor = color;
		}

		glm::vec3 Material_::getEmissionColor() const
		{
			return _emissionColor;
		}

		void Material_::setDiffuseMap(Texture* map, const bool use)
		{
			_diffuseMap = std::shared_ptr<Texture>(map);
			_useDiffuseMap = use;
		}

		Texture* Material_::getDiffuseMap() const
		{
			return _diffuseMap.get();
		}

		void Material_::setSpecularMap(Texture* map, const bool use)
		{
			_specularMap = std::shared_ptr<Texture>(map);
			_useSpecularMap = use;
		}

		Texture* Material_::getSpecularMap() const
		{
			return _specularMap.get();
		}

		void Material_::setEmissionMap(Texture* map, const bool use)
		{
			_emissionMap = std::shared_ptr<Texture>(map);
			_useEmissionMap = use;
		}

		Texture* Material_::getEmissionMap() const
		{
			return _emissionMap.get();
		}

		void Material_::useDiffuseMap(const bool use)
		{
			_useDiffuseMap = use;
		}

		void Material_::useSpecularMap(const bool use)
		{
			_useSpecularMap = use;
		}

		void Material_::useEmissionMap(const bool use)
		{
			_useEmissionMap = use;
		}

		void Material_::useEmission(const bool use)
		{
			_useEmission = use;
		}

		bool Material_::isDiffuseMapUsed() const
		{
			return _useDiffuseMap;
		}

		bool Material_::isSpecularMapUsed() const
		{
			return _useSpecularMap;
		}

		bool Material_::isEmissionMapUsed() const
		{
			return _useEmissionMap;
		}

		bool Material_::isEmissionUsed() const
		{
			return _useEmission;
		}

		void Material_::setDiffuseColor(const glm::vec3 color)
		{
			_diffuseColor = color;
		}

		glm::vec3 Material_::getDiffuseColor() const
		{
			return _diffuseColor;
		}

		void Material_::setSpecularColor(const glm::vec3 color)
		{
			_specularColor = color;
		}
	}
}
