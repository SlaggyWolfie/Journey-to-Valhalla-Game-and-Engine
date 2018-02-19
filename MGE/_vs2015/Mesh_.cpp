#include "Mesh_.hpp"
#include <utility>
#include <iostream>
#include <string>

namespace Engine
{
	namespace Rendering
	{
		Mesh_* Mesh_::load(const std::string& path)
		{
			std::cout << "Load " + path << std::endl;
			return nullptr;
		}

		Mesh_::Mesh_(std::vector<Vertex>  vertices, std::vector<int>  indices) :
			vertices(std::move(vertices)), indices(std::move(indices))
		{
		}
	}
}
