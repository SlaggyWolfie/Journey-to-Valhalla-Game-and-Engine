#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class Shader
		{
		public:
			Shader();
			~Shader() = default;

			//GL_VERTEX_SHADER / GL_FRAGMENT_SHADER
			void addShader(GLuint shaderType, const std::string& shaderPath);
			//link and compile all added shaders
			void finalize();
			//tell opengl this is now the current shader program
			void bind() const;
			static void unbind() ;

			GLuint getUniformLocation(const std::string& name) const;
			GLuint getAttributeLocation(const std::string& name) const;

		private:
			GLint _programID;

			std::vector<GLuint> _shaderIDs;

			std::string readFile(const std::string &shaderPath) const;
			GLuint compileShader(GLuint shaderType, const std::string &shaderSource);
		};
	}
}

#endif //SHADER_HPP