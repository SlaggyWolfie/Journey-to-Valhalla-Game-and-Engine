#include "Mesh_.hpp"
#include <utility>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "Camera_.hpp"

namespace Engine
{
	namespace Rendering
	{
		Vertex::Vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 uv, const glm::vec3 tangent) :
			position(position), normal(normal), textureCoordinate(uv), tangent(tangent)
		{

		}

		void Mesh_::addVertex(const Vertex vertex)
		{
			_vertices.push_back(vertex);
		}

		void Mesh_::addVertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 uv, const glm::vec3 tangent)
		{
			_vertices.emplace_back(position, normal, uv, tangent);
		}

		void Mesh_::addIndex(const int newIndex)
		{
			_indices.push_back(newIndex);
		}

		void Mesh_::removeVertex(const int index)
		{
			//_vertices.erase(_vertices.begin() + index);
			//_vertices.erase
			_vertices.erase(std::next(_vertices.begin(), index));
		}

		void Mesh_::removeIndex(const int index)
		{
			_indices.erase(std::next(_indices.begin(), index));
		}

		Vertex Mesh_::getVertex(const int index) const
		{
			return _vertices[index];
		}

		int Mesh_::getIndex(const int index) const
		{
			return _indices[index];
		}

		int Mesh_::getVertexCount() const
		{
			return static_cast<int>(_vertices.size());
		}

		int Mesh_::getIndexCount() const
		{
			return static_cast<int>(_indices.size());
		}

		std::vector<Vertex> Mesh_::getAllVerticesList() const
		{
			return _vertices;
		}

		std::vector<int> Mesh_::getAllIndicesList() const
		{
			return _indices;
		}

		Vertex* Mesh_::getAllVertices() const
		{
			const auto size = static_cast<unsigned int>(_vertices.size());
			Vertex* vertexArray = new Vertex[size];
			std::copy(_vertices.begin(), _vertices.end(), vertexArray);
			return vertexArray;
		}

		int* Mesh_::getAllIndices() const
		{
			const auto size = static_cast<unsigned int>(_indices.size());
			int* indexArray = new int[size];
			std::copy(_indices.begin(), _indices.end(), indexArray);
			return indexArray;
		}

		Mesh_::Mesh_(std::vector<Vertex>  vertices, std::vector<int>  indices) : VAO(0),
			_vertices(std::move(vertices)),
			_indices(std::move(indices)), VBO(0), EBO(0),
			_bufferVertexPositions(0),
			_bufferVertexNormals(0),
			_bufferVertexUVs(0), _bufferIndex(0)
		{
		}

		void Mesh_::prewake()
		{
			generateBuffers2();
			//generateBuffers();
		}

		void Mesh_::destroy()
		{
			delete this;
		}

		bool Mesh_::isUniquePerGameObject()
		{
			return true;
		}

		void Mesh_::stream(const GLint& verticesAttribute, const GLint& normalsAttribute, const GLint& UVsAttribute) const
		{
			if (verticesAttribute != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexPositions);
				glEnableVertexAttribArray(verticesAttribute);
				glVertexAttribPointer(verticesAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			}

			if (normalsAttribute != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexNormals);
				glEnableVertexAttribArray(normalsAttribute);
				glVertexAttribPointer(normalsAttribute, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
			}

			if (UVsAttribute != -1)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexUVs);
				glEnableVertexAttribArray(UVsAttribute);
				glVertexAttribPointer(UVsAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferIndex);

			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, static_cast<GLvoid*>(nullptr));

			// no current buffer, to avoid mishaps, very important for performance

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//fix for serious performance issue
			if (UVsAttribute != -1) glDisableVertexAttribArray(UVsAttribute);
			if (normalsAttribute != -1) glDisableVertexAttribArray(normalsAttribute);
			if (verticesAttribute != -1) glDisableVertexAttribArray(verticesAttribute);
		}

		void Mesh_::stream2(const GLint& verticesAttribute, const GLint& normalsAttribute, const GLint& UVsAttribute) const
		{
			// draw mesh
			glBindVertexArray(VAO);
			//glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			////glBindVertexArray(0);
		}

		void Mesh_::generateBuffers()
		{
			glGenBuffers(1, &_bufferIndex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferIndex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &_bufferVertexPositions);
			glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexPositions);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0].position, GL_STATIC_DRAW);

			glGenBuffers(1, &_bufferVertexNormals);
			glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexNormals);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0].normal, GL_STATIC_DRAW);

			glGenBuffers(1, &_bufferVertexUVs);
			glBindBuffer(GL_ARRAY_BUFFER, _bufferVertexUVs);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec2), &_vertices[0].textureCoordinate, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void Mesh_::generateBuffers2()
		{
			// create buffers/arrays
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// A great thing about structs is that their memory layout is sequential for all its items.
			// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

			// set the vertex attribute pointers
			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coordinates
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));

			glBindVertexArray(0);
		}
	}
}
