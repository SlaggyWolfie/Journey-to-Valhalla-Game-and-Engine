#pragma once
#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "Component.hpp"
#include <glm/glm.hpp>
#include <memory>
#include "NeedsGLSetup.hpp"

//#include "mge/core/ShaderProgram.hpp"

namespace Engine
{
	namespace Rendering
	{
		class Shader;
		class Texture;

		class Material_ : public Core::Component
		{
		public:
			Material_();
			virtual ~Material_();
			Shader* getShader();

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

			void useDiffuseMap(bool use = true);
			void useSpecularMap(bool use = true);
			void useEmissionMap(bool use = true);
			void useEmission(bool use = true);

			bool isDiffuseMapUsed() const;
			bool isSpecularMapUsed() const;
			bool isEmissionMapUsed() const;
			bool isEmissionUsed() const;

		protected:
			//void setupGL() override;
			static std::shared_ptr<Shader> _shader;

			glm::vec3 _diffuseColor;
			glm::vec3 _specularColor;
			glm::vec3 _emissionColor;

			std::shared_ptr<Texture> _diffuseMap;
			std::shared_ptr<Texture> _specularMap;
			std::shared_ptr<Texture> _emissionMap;

			bool _useDiffuseMap;
			bool _useSpecularMap;
			bool _useEmissionMap;
			bool _useEmission;
		};
	}
}

#endif //MATERIAL_HPP