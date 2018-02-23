#pragma once
#ifndef MODEL_HPP
#define MODEL_HPP

//#include "Mesh_.hpp"
#include <string>
#include <vector>
//#include <assimp/>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Renderer_.hpp"

namespace Engine
{
	namespace Core { class GameObject_; }
	namespace Rendering { class Mesh_; class Material_; }

	class Model
	{
	public:
		static Core::GameObject_* loadModel(const std::string& path);
	private:
		static Core::GameObject_* processNode(aiNode *node, const aiScene *scene);
		static Rendering::RenderQueue processMesh(aiMesh *mesh, const aiScene *scene, Rendering::Mesh_* &outMesh, Rendering::Material_* &outMaterial);
		static Rendering::Material_* loadMaterial(aiMaterial *material);
		static glm::mat4 convert(aiMatrix4x4 aiMatrix);
		static int _recursionLevel;
		static void print(const std::string& message);
	};
}

#endif