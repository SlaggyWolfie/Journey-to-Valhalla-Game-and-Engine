#pragma once
#ifndef SHADOW_MAP_HPP
#define SHADOW_MAP_HPP

//#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>

namespace Engine
{
	namespace Rendering
	{
		class Light_;
		class Shader;

		class ShadowMap
		{
		public:
			ShadowMap();
			~ShadowMap();
			void bindFramebuffer() const;
			static void unbindFramebuffer();
			void bindTexture() const;
			static void unbindTexture();
			//void renderMap();
			int getWidth() const;
			int getHeight() const;
			GLuint getTextureID() const;
			GLuint getFramebuffer() const;
			Shader* getShader() const;
			void pushLightSpaceMatrix(glm::mat4 lightSpaceMatrix) const;
			void pushModelMatrix(glm::mat4 modelMatrix) const;
		private:
			std::unique_ptr<Shader> _shader = nullptr;
			//Light_* _light = nullptr;

			static GLuint _uniformLightSpaceMatrix;
			static GLuint _uniformModelMatrix;

			GLuint _fbo = 0;
			GLuint _textureID = 0;
			int _width = 0;
			int _height = 0;

			void initializeShader();
			void generateFramebuffer();
			void generateMap();
			void bindMapToFramebuffer() const;
		};
	}
}

#endif //SHADOW_MAP_HPP