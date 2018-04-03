#include "Material_.hpp"
#include "Shader.hpp"
#include "Texture_.hpp"

namespace Engine
{
	namespace Rendering
	{
		std::unique_ptr<Shader> Material_::_shader = nullptr;

		Material_::Material_(const std::string& path) :
			_diffuseColor(glm::vec3(1)), _specularColor(glm::vec3(1)), _emissionColor(glm::vec3(0, 1, 0)),
			_diffuseMap(nullptr), _specularMap(nullptr), _emissionMap(nullptr),
			_useDiffuseMap(false), _useSpecularMap(false), _useEmissionMap(false), _useEmission(false),
			_shininess(1), _specularStrength(0.0f), _emissionStrength(1), _diffuseStrength(1)
		{
			initializeShader(path);
		}

		Material_::~Material_()
		{
			_shader = nullptr;
			//_diffuseMap = nullptr;
			//_specularMap = nullptr;
			//_emissionMap = nullptr;
		}

		Shader* Material_::getShader() const
		{
			//return _shader;
			return _shader.get();
		}

		glm::vec3 Material_::getSpecularColor() const
		{
			return _specularColor;
		}

		void Material_::setEmissionColor(const glm::vec3 color)
		{
			_emissionColor = glm::clamp(color, glm::vec3(0), glm::vec3(1));
		}

		glm::vec3 Material_::getEmissionColor() const
		{
			return _emissionColor;
		}

		void Material_::setDiffuseMap(Texture_* map, const bool use)
		{
			_diffuseMap = map;
			_useDiffuseMap = use;
		}

		Texture_* Material_::getDiffuseMap()
		{
			if (_diffuseMap == nullptr) _diffuseMap =
				Texture_::loadDefault(
					_diffuseColor.r,
					_diffuseColor.g,
					_diffuseColor.b);

			return _diffuseMap;
		}

		void Material_::setSpecularMap(Texture_* map, const bool use)
		{
			_specularMap = map;
			_useSpecularMap = use;
		}

		Texture_* Material_::getSpecularMap()
		{
			if (_specularMap == nullptr) _specularMap =
				Texture_::loadDefault(
					_specularColor.r,
					_specularColor.g,
					_specularColor.b,
				TextureType::Specular);

			return _specularMap;
		}

		void Material_::setEmissionMap(Texture_* map, const bool use)
		{
			_emissionMap = map;
			_useEmissionMap = use;
		}

		Texture_* Material_::getEmissionMap()
		{
			if (_emissionMap == nullptr) _emissionMap =
				Texture_::loadDefault(
					_specularColor.r,
					_specularColor.g,
					_specularColor.b,
					TextureType::Emission);

			return _emissionMap;
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

		void Material_::setShininess(const float shininess)
		{
			_shininess = shininess;
		}

		void Material_::setSpecularStrength(const float strength)
		{
			_specularStrength = glm::clamp(strength, 0.0f, 1.0f);
		}

		void Material_::setEmissionStrength(const float strength)
		{
			_emissionStrength = glm::clamp(strength, 0.0f, 1.0f);
		}

		void Material_::setDiffuseStrength(const float strength)
		{
			_diffuseStrength = glm::clamp(strength, 0.0f, 1.0f);
		}

		float Material_::getShininess() const
		{
			return _shininess;
		}

		float Material_::getSpecularStrength() const
		{
			return _specularStrength;
		}

		float Material_::getEmissionStrength() const
		{
			return _emissionStrength;
		}

		float Material_::getDiffuseStrength() const
		{
			return _diffuseStrength;
		}

		void Material_::initializeShader(const std::string& path)
		{
			if (_shader != nullptr) return;
			_shader = std::make_unique<Shader>();
			_shader->addShader(GL_VERTEX_SHADER, path + ".vs");
			_shader->addShader(GL_FRAGMENT_SHADER, path + ".fs");
			_shader->finalize();
		}

		void Material_::setDiffuseColor(const glm::vec3 color)
		{
			_diffuseColor = glm::clamp(color, glm::vec3(0), glm::vec3(1));
		}

		glm::vec3 Material_::getDiffuseColor() const
		{
			return _diffuseColor;
		}

		void Material_::setSpecularColor(const glm::vec3 color)
		{
			_specularColor = glm::clamp(color, glm::vec3(0), glm::vec3(1));
		}
	}
}
