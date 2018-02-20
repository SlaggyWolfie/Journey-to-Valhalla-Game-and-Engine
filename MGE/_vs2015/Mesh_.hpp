#pragma once
#ifndef MESH__HPP
#define MESH__HPP
#include "Component.hpp"
#include <glm/glm.hpp>
#include "NeedsGLSetup.hpp"
#include <vector>

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
			friend class Model;
		public:
			Mesh_() = default;
			~Mesh_() = default;
			Mesh_(const Mesh_& other) = default;
			Mesh_& operator=(const Mesh_& other) = default;

			static Mesh_ * load(const std::string& path);

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
		private:
			Mesh_(std::vector<Vertex> vertices, std::vector<int> indices);

			std::vector<Vertex> _vertices;
			std::vector<int> _indices;
		};
	}
}

#endif //MESH__HPP