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
		};

		class Mesh_ : public Core::Component
		{
			friend class Model;
		public:
			Mesh_() = default;
			~Mesh_() = default;
			Mesh_(const Mesh_& other) = default;
			Mesh_& operator=(const Mesh_& other) = default;

			std::vector<Vertex> vertices;
			std::vector<int> indices;

			static Mesh_ * load(const std::string& path);

		private:
			Mesh_(std::vector<Vertex>  vertices, std::vector<int>  indices);
		};
	}
}

#endif //MESH__HPP