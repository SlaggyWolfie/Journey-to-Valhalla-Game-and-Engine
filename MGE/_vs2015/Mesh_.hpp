#pragma once
#ifndef MESH__HPP
#define MESH__HPP
#include "Component.hpp"
#include <glm/glm.hpp>

namespace Engine
{
	namespace Rendering
	{
		struct Vertex
		{
		public:
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 textureCoordinate;
		};

		class Mesh_ : Core::Component
		{
			
		};
	}
}

#endif //MESH__HPP