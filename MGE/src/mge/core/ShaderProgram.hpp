#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <vector>
#include <string>

/**
 * Generic shader program to which you can add separate shaders.
 * Nice exercise for the reader: make it possible to add shaders by passing in the code as a string instead of through a file.
 *
 * Usage:
 *  -create shader program
 *  -add shaders
 *  -finalize shader program
 *  -use shader program
 *
 * See the example material classes for a demo.
 */
class ShaderProgram
{
    public:
		ShaderProgram();
		virtual ~ShaderProgram();

        //add a shader of a specific type (eg GL_VERTEX_SHADER / GL_FRAGMENT_SHADER)
		void addShader(GLuint pShaderType, const std::string& pShaderPath);
		//link and compile all added shaders
		void finalize();
		//tell opengl this is now the current shader program
		void use();

        //get access to uniforms within the shader
		GLuint getUniformLocation (const std::string& pName);
		//get access to attributes within the shader
		GLuint getAttribLocation (const std::string& pName);

	private:
	    GLint _programId;

	    std::vector<GLuint> _shaderIds;

		std::string _readFile(const std::string &pShaderPath);
		GLuint _compileShader (GLuint pShaderType, const std::string &pShaderSource);
};

#endif // SHADERPROGRAM_HPP

