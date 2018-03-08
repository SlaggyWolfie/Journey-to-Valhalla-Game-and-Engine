#pragma once
#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "Component.hpp"
#include <glm/glm.hpp>
#include <memory>
#include "NeedsGLSetup.hpp"
#include <string>

//#include "mge/core/ShaderProgram.hpp"

namespace Engine
{
	namespace Rendering
	{
		class Shader;
		class Texture_;

		class Material_ : public Core::Component
		{
		public:
			Material_(const std::string& path = "shaders/hidden/default");
			virtual ~Material_();
			Shader* getShader() const;

			void setDiffuseColor(glm::vec3 color);
			glm::vec3 getDiffuseColor() const;
			void setSpecularColor(glm::vec3 color);
			glm::vec3 getSpecularColor() const;
			void setEmissionColor(glm::vec3 color);
			glm::vec3 getEmissionColor() const;

			void setDiffuseMap(Texture_* map, bool use = true);
			Texture_* getDiffuseMap();
			void setSpecularMap(Texture_* map, bool use = true);
			Texture_* getSpecularMap();
			void setEmissionMap(Texture_* map, bool use = true);
			Texture_* getEmissionMap();

			void useDiffuseMap(bool use = true);
			void useSpecularMap(bool use = true);
			void useEmissionMap(bool use = true);
			void useEmission(bool use = true);

			bool isDiffuseMapUsed() const;
			bool isSpecularMapUsed() const;
			bool isEmissionMapUsed() const;
			bool isEmissionUsed() const;

			void setShininess(float shininess);
			void setSpecularStrength(float strength);
			void setEmissionStrength(float strength);
			void setDiffuseStrength(float strength);

			float getShininess() const;
			float getSpecularStrength() const;
			float getEmissionStrength() const;
			float getDiffuseStrength() const;

		protected:
			//void setupGL() override;
			static std::unique_ptr<Shader> _shader;
			//std::unique_ptr<Shader> _shader;
			void initializeShader(const std::string& path);

			glm::vec3 _diffuseColor;
			glm::vec3 _specularColor;
			glm::vec3 _emissionColor;

			Texture_* _diffuseMap;
			Texture_* _specularMap;
			Texture_* _emissionMap;

			bool _useDiffuseMap;
			bool _useSpecularMap;
			bool _useEmissionMap;
			bool _useEmission;

			float _shininess;
			float _specularStrength;
			float _emissionStrength;
			float _diffuseStrength;

			int RGB_to_Hex(glm::vec3 color) const;
		};
	}
}

#endif //MATERIAL_HPP