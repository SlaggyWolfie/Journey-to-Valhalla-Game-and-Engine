#ifndef MESH_GENERATION_HPP
#define MESH_GENERATION_HPP

#include "mge/core/Mesh.hpp"

class Texture;

namespace MyGame
{
	class MeshGeneration : public Mesh
	{
	public:
		MeshGeneration();
		~MeshGeneration();

		static Mesh* GenerateMesh(Texture* heightmap, int widthResolution = 512, int lengthResolution = 512);

	private:
		MeshGeneration(const MeshGeneration&);
		MeshGeneration operator=(const MeshGeneration&);
	};
}

#endif //MESH_GENERATION_HPP