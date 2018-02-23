#include "Shader.hpp"
#include <iostream>
#include <ostream>
#include <fstream>

namespace Engine
{
	namespace Rendering
	{
		Shader::Shader() :_programID(0)
		{
			//why does opengl use glCreateProgram and not glGenProgram (1, &_programID)? Who knows:) *shrugs*
			_programID = glCreateProgram();
			std::cout << std::endl << "Program created with id:" << _programID << std::endl;
		}

		void Shader::addShader(const GLuint shaderType, const std::string &shaderPath)
		{
			const std::string shaderCode = readFile(shaderPath);
			const GLuint shaderID = compileShader(shaderType, shaderCode);

			if (shaderID != 0)
				_shaderIDs.push_back(shaderID);
		}

		std::string Shader::readFile(const std::string& shaderPath) const
		{
			std::string contents;
			std::ifstream file(shaderPath, std::ios::in);
			if (file.is_open())
			{
				std::cout << "Reading shader file... " << shaderPath << std::endl;
				std::string line;
				while (getline(file, line)) contents += "\n" + line;
				file.close();
			}
			else {
				std::cout << "Error reading shader " << shaderPath << std::endl;
				contents = "";
			}
			return contents;
		}

		// compile the code, and detect errors.
		GLuint Shader::compileShader(GLuint shaderType, const std::string& shaderSource)
		{
			std::cout << "Compiling shader... " << std::endl;
			const char * sourcePointer = shaderSource.c_str();
			const GLuint shaderID = glCreateShader(shaderType);
			glShaderSource(shaderID, 1, &sourcePointer, nullptr);
			glCompileShader(shaderID);

			GLint compilerResult = GL_FALSE;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilerResult);

			if (!compilerResult)
			{
				// get error message
				std::cout << "Shader error:" << std::endl;
				int infoLogLength;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
				char* errorMessage = new char[infoLogLength + 1];
				glGetShaderInfoLog(shaderID, infoLogLength, nullptr, errorMessage);
				std::cout << errorMessage << std::endl << std::endl;
				delete[] errorMessage;
				return 0;
			}

			std::cout << "Shader compiled ok." << std::endl;
			return shaderID;
		}

		void Shader::finalize()
		{
			for (auto shaderID : _shaderIDs)
			{
				glAttachShader(_programID, shaderID);
			}

			glLinkProgram(_programID);

			// Check the program
			GLint linkResult = GL_FALSE;
			glGetProgramiv(_programID, GL_LINK_STATUS, &linkResult);

			if (linkResult)
			{
				std::cout << "Program linked ok." << std::endl << std::endl;
			}
			else
			{ // error, show message
				std::cout << "Program error:" << std::endl;

				int infoLogLength;
				glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
				char* errorMessage = new char[infoLogLength + 1];
				glGetProgramInfoLog(_programID, infoLogLength, nullptr, errorMessage);
				std::cout << errorMessage << std::endl << std::endl;
				delete[] errorMessage;
			}

			for (auto shaderID : _shaderIDs)
			{
				glDeleteShader(shaderID);
			}
		}

		GLuint Shader::getUniformLocation(const std::string& name) const
		{
			return glGetUniformLocation(_programID, name.c_str());
		}

		GLuint Shader::getAttributeLocation(const std::string& name) const
		{
			return glGetAttribLocation(_programID, name.c_str());
		}

		void Shader::bind() const
		{
			glUseProgram(_programID);
		}

		void Shader::unbind()
		{
			glUseProgram(0);
		}
	}
}
