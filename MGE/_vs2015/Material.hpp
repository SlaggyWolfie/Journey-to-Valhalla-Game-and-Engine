#pragma once
#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "Component.hpp"
#include <glm/glm.hpp>
#include <memory>
#include "mge/core/ShaderProgram.hpp"

namespace Engine
{
	namespace Rendering
	{
		class Texture;

		class Material : public Core::Component
		{
		public:
			static ShaderProgram* getShader();

			void setDiffuseColor(glm::vec3 color);
			glm::vec3 getDiffuseColor() const;
			void setSpecularColor(glm::vec3 color);
			glm::vec3 getSpecularColor() const;
			void setEmissionColor(glm::vec3 color);
			glm::vec3 getEmissionColor() const;

			void setDiffuseMap(Texture* map, bool use = true);
			Texture* getDiffuseMap() const;
			void setSpecularMap(Texture* map, bool use = true);
			Texture* getSpecularMap() const;
			void setEmissionMap(Texture* map, bool use = true);
			Texture* getEmissionMap() const;

			bool useDiffuseMap(bool use = true);
			bool useSpecularMap(bool use = true);
			bool useEmissionMap(bool use = true);
			bool useEmission(bool use = true);

			void bind();
			void unbind();

		protected:
			static std::shared_ptr<ShaderProgram> _shader;

			glm::vec3 _diffuseColor;
			glm::vec3 _specularColor;
			glm::vec3 _emissionColor;

			std::unique_ptr<Texture> _diffuseMap;
			std::unique_ptr<Texture> _specularMap;
			std::unique_ptr<Texture> _emissionMap;
		};
	}
}

#endif //MATERIAL_HPP