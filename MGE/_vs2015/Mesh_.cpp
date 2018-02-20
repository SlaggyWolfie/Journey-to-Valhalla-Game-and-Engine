#include "Mesh_.hpp"
#include <utility>
#include <iostream>
#include <string>

namespace Engine
{
	namespace Rendering
	{
		Vertex::Vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 uv) :
			position(position), normal(normal), textureCoordinate(uv)
		{

		}

		Mesh_* Mesh_::load(const std::string& path)
		{
			std::cout << "Load " + path << std::endl;
			return nullptr;
		}

		void Mesh_::addVertex(const Vertex vertex)
		{
			_vertices.push_back(vertex);
		}

		void Mesh_::addVertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 uv)
		{
			_vertices.emplace_back(position, normal, uv);
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

		Mesh_::Mesh_(std::vector<Vertex>  vertices, std::vector<int>  indices) :
			_vertices(std::move(vertices)), _indices(std::move(indices))
		{
		}
	}
}
