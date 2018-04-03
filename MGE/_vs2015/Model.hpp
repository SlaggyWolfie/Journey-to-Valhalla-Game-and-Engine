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
#include "Mesh_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include "Material_.hpp"
#include "Renderer_.hpp"

namespace Engine
{
	//namespace Core { class GameObject_; }
	//namespace Rendering { class Mesh_; class Material_; class Renderer_ :: public Core::Compone }

	struct ShallowMesh
	{
		ShallowMesh* parent = nullptr;
		Rendering::Mesh_* mesh = nullptr;
		Rendering::Material_* material = nullptr;
		Rendering::Renderer_* renderer = nullptr;

		ShallowMesh() = default;
		~ShallowMesh() = default;
	};

	class Model
	{
	public:
		static Core::GameObject_* loadModel(const std::string& path);
		//static ShallowMesh* loadModelShallow(const std::string& path);
		static void debug(bool debug);
		static bool clipPaths;
	private:
		static Core::GameObject_* processNode(aiNode *node, const aiScene *scene);
		//static ShallowMesh* processNodeShallow(aiNode *node, const aiScene *scene);
		static Rendering::RenderQueue processMesh(aiMesh *mesh, const aiScene *scene, Rendering::Mesh_* &outMesh, Rendering::Material_* &outMaterial);
		static Rendering::Material_* loadMaterial(aiMaterial *material);
		static glm::mat4 convert(aiMatrix4x4 aiMatrix);
		static int _recursionLevel;
		static bool _debug;
		static void print(const std::string& message, bool recursive = true);
		static double _scale;
		static std::string removeParentFolders(std::string path);
		static glm::vec3 _getTranslation(const glm::mat4& matrix);
		static glm::quat _getOrientation(const glm::mat4& matrix);
		static glm::vec3 _getScale(const glm::mat4& matrix);
		static glm::vec3 _getSkew(const glm::mat4& matrix);
		static glm::vec4 _getPerspective(const glm::mat4& matrix);
	};
}

#endif