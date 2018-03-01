#include "ShadowMap.hpp"
#include "Light_.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <glm.hpp>

namespace Engine
{
	namespace Rendering
	{
		GLuint ShadowMap::_uniformLightSpaceMatrix = 0;
		GLuint ShadowMap::_uniformModelMatrix = 0;

		ShadowMap::ShadowMap() : _shader(nullptr), _fbo(0), _textureID(0), _width(1024), _height(1024)
		{
			initializeShader();
			generateFramebuffer();
			generateMap();
			bindMapToFramebuffer();

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer error with shadow map!" << std::endl;
		}

		ShadowMap::~ShadowMap()
		{
			glDeleteTextures(1, &_textureID);
			glDeleteFramebuffers(1, &_fbo);
			_shader = nullptr;
		}

		void ShadowMap::bindFramebuffer() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		}

		void ShadowMap::unbindFramebuffer()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void ShadowMap::bindTexture() const
		{
			glBindTexture(GL_TEXTURE_2D, _textureID);
		}

		void ShadowMap::unbindTexture()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		int ShadowMap::getWidth() const
		{
			return _width;
		}

		int ShadowMap::getHeight() const
		{
			return _height;
		}

		GLuint ShadowMap::getTextureID() const
		{
			return _textureID;
		}

		GLuint ShadowMap::getFramebuffer() const
		{
			return _fbo;
		}

		Shader* ShadowMap::getShader() const
		{
			return _shader.get();
		}

		void ShadowMap::pushLightSpaceMatrix(glm::mat4 lightSpaceMatrix) const
		{
			glUniformMatrix4fv(_uniformLightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
			//std::cout << glm::to_string(lightSpaceMatrix) << std::endl;
		}

		void ShadowMap::pushModelMatrix(glm::mat4 modelMatrix) const
		{
			glUniformMatrix4fv(_uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		}

		void ShadowMap::initializeShader()
		{
			const std::string path = "shaders/hidden/shadowmap";
			_shader = std::make_unique<Shader>();
			_shader->addShader(GL_VERTEX_SHADER, path + ".vs");
			_shader->addShader(GL_FRAGMENT_SHADER, path + ".fs");
			_shader->finalize();

			_uniformLightSpaceMatrix = _shader->getUniformLocation("lightSpaceMatrix");
			_uniformModelMatrix = _shader->getUniformLocation("modelMatrix");
		}

		void ShadowMap::generateFramebuffer()
		{
			glGenFramebuffers(1, &_fbo);
		}

		void ShadowMap::generateMap()
		{
			glGenTextures(1, &_textureID);
			glBindTexture(GL_TEXTURE_2D, _textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		}

		void ShadowMap::bindMapToFramebuffer() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _textureID, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}
