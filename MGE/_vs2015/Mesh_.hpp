#pragma once
#ifndef MESH__HPP
#define MESH__HPP
#include "Component.hpp"
#include <glm/glm.hpp>
#include "NeedsGLSetup.hpp"
#include <vector>
#include "GL/glew.h"
#include "Model.hpp"

namespace Engine
{
	namespace Rendering
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 textureCoordinate;

			Vertex() = default;
			Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv);
			//Vertex(const Vertex& other) = default;
			//Vertex& operator= (const Vertex& other) = default;
			//Vertex(const Vertex&& other) = default;
			//~Vertex() = default;
		};

		class Mesh_ : public Core::Component
		{
		public:
			Mesh_() = default;
			~Mesh_() = default;
			Mesh_(std::vector<Vertex> vertices, std::vector<int> indices);
			Mesh_(const Mesh_& other) = default;
			Mesh_& operator=(const Mesh_& other) = default;

			//static Mesh_ * load(const std::string& path);
			void stream(const GLint& verticesAttribute, const GLint & normalsAttribute, const GLint& UVsAttribute) const;
			void stream2(const GLint& verticesAttribute, const GLint & normalsAttribute, const GLint& UVsAttribute) const;
			void generateBuffers();
			void generateBuffers2();

			void addVertex(Vertex vertex);
			void addVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv);
			void addIndex(int newIndex);

			void removeVertex(int index);
			void removeIndex(int index);

			Vertex getVertex(int index) const;
			int getIndex(int index) const;

			int getVertexCount() const;
			int getIndexCount() const;

			std::vector<Vertex> getAllVerticesList() const;
			std::vector<int> getAllIndicesList() const;

			Vertex* getAllVertices() const;
			int* getAllIndices() const;

			unsigned int VAO;

		protected:
			void prewake() override;
		public:
			void destroy() override;
		protected:
			bool isUniquePerGameObject() override;
		private:
			std::vector<Vertex> _vertices;
			std::vector<int> _indices;
			unsigned int VBO, EBO;

			GLuint _bufferVertexPositions;
			GLuint _bufferVertexNormals;
			GLuint _bufferVertexUVs;
			GLuint _bufferIndex;
		};
	}
}

#endif //MESH__HPP