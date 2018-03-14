#include "Model.hpp"
#include "Texture_.hpp"
#include <assimp/postprocess.h>
#include <iostream>
#include <glm/gtx/matrix_decompose.inl>

namespace Engine
{
	int Model::_recursionLevel = 0;
	double Model::_scale = 1;
	bool Model::_debug = false;
	bool Model::clipPaths = false;

	Core::GameObject_* Model::loadModel(const std::string& path)
	{
		//= false;
		std::cout << "Loading model at path " + path << std::endl;
		Assimp::Importer import;
		import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, true);
		import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_CAMERAS, false);
		import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_ANIMATIONS, false);
		import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_LIGHTS, false);
		const aiScene *scene = import.ReadFile(path,
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals
			| aiProcess_ImproveCacheLocality | aiProcess_GenUVCoords| aiProcess_FindInvalidData
			| aiProcess_SplitLargeMeshes | aiProcess_ValidateDataStructure | aiProcess_CalcTangentSpace
			| aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return nullptr;
		}

		if (scene->mMetaData)
			scene->mMetaData->Get("UnitScaleFactor", _scale);
		//this->path = path.substr(0, path.find_last_of('/'));
		//if (path.find(".obj") != std::string::npos) _scale = 100;

		Core::GameObject_* go = processNode(scene->mRootNode, scene);
		std::cout << path << std::endl;
		std::cout << "Loaded model at path " + path << std::endl;
		_recursionLevel = 0;
		_scale = 1;
		
		return go;
	}

	Core::GameObject_* Model::processNode(aiNode *node, const aiScene *scene)
	{
		_recursionLevel++;
		print(std::string("Processing node ") + node->mName.C_Str());
		Core::GameObject_* gameObject;// = new GameObject_(node->mName.C_Str(), "");
		// process all the node's meshes (if any)
		print(std::string("Has ") + std::to_string(node->mNumMeshes) + " amount of meshes.");
		print(std::string("Has ") + std::to_string(node->mNumChildren) + " amount of children.");

		bool skip = false;
		// then do the same for each of its children
		//if (node->mNumChildren == 1 && node == scene->mRootNode)
		//{
		//	gameObject = processNode(node->mChildren[0], scene);
		//	skip = true;
		//}
		//else
		gameObject = new Core::GameObject_(node->mName.C_Str(), "");

		Engine::Core::Transform* transform = gameObject->getTransform();
		if (node != scene->mRootNode)
		{
			const auto something = convert(node->mTransformation);
			transform->setLocalMatrix4X4(something);
		}

		transform->setLocalScale(transform->getLocalScale() * glm::vec3(1 / _scale));
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			Rendering::Material_* mymat = nullptr;
			Rendering::Mesh_* mymesh = nullptr;
			const Rendering::RenderQueue queue = processMesh(mesh, scene, mymesh, mymat);
			//std::cout << "Wrong" << std::endl;
			//std::cout << "My Mesh: " + std::to_string(mymesh != nullptr) << std::endl;
			//std::cout << "My Material: " + std::to_string(mymat != nullptr) << std::endl;
			Core::GameObject_* meshGO = new Core::GameObject_("Mesh_" + std::string(mesh->mName.C_Str()), "");
			meshGO->addComponent(mymesh);
			meshGO->addComponent(mymat);
			meshGO->addComponent(new Rendering::Renderer_(mymat, mymesh, queue, true));
			meshGO->getTransform()->setParent(gameObject->getTransform(), false);
			//go->addComponent<Renderer_>();
		}

		if (!skip)
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				Core::GameObject_* childGO = processNode(node->mChildren[i], scene);
				childGO->getTransform()->setParent(gameObject->getTransform(), true);
			}

		print(std::string("Processed node ") + node->mName.C_Str());
		//std::cout << "Processed node " << node->mName.C_Str() << std::endl;

		_recursionLevel--;
		return gameObject;
	}

	Rendering::RenderQueue Model::processMesh(aiMesh *mesh, const aiScene *scene, Rendering::Mesh_*& outMesh, Rendering::Material_*& outMaterial)
	{
		std::vector<Rendering::Vertex> vertices;
		std::vector<int> indices;
		//std::vector<Texture_*> textures;
		//outMaterial = new Material_();
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Rendering::Vertex vertex;
			// process vertex positions, normals and texture coordinates
			glm::vec3 vector;

			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;

			if (mesh->mTextureCoords[0]) // do you even UV, bruh?
			{
				glm::vec2 uv;
				uv.x = mesh->mTextureCoords[0][i].x;
				uv.y = mesh->mTextureCoords[0][i].y;
				vertex.textureCoordinate = uv;
			}
			else
				vertex.textureCoordinate = glm::vec2(0.0f, 0.0f);

			//std::cout << "Vertex " + std::to_string(i) + " " + glm::to_string(vertex.position) + " " << std::endl;
			vertices.push_back(vertex);
		}
		//std::cout << std::endl;

		// process indices
		int index = 0;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				index++;
				//std::cout << "Index " + std::to_string(index) + " " + std::to_string(face.mIndices[j]) + " " << std::endl;
				indices.push_back(face.mIndices[j]);
			}
		}
		print(std::string("Processing mesh") + mesh->mName.C_Str() + " with " +
			std::to_string(mesh->mNumVertices) + " vertices with " + std::to_string(index) + " indices.");
		//std::cout << std::endl;

		// process material
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		outMaterial = loadMaterial(material);
		outMesh = new Rendering::Mesh_(vertices, indices);

		Rendering::RenderQueue queue = Rendering::RenderQueue::Transparent;
		float opacity;
		material->Get(AI_MATKEY_OPACITY, opacity);
		if (glm::epsilonEqual(opacity, 1.0f, glm::epsilon<float>()))
			queue = Rendering::RenderQueue::Opaque;

		//std::cout << "Out Mesh: " + std::to_string(outMesh != nullptr) << std::endl;
		//std::cout << "Out Material: " + std::to_string(outMaterial != nullptr) << std::endl;
		print(std::string("Processing mesh") + mesh->mName.C_Str());
		return queue;
	}

	Rendering::Material_* Model::loadMaterial(aiMaterial* material)
	{
		_recursionLevel++;
		//float opacity;
		//material->Get(AI_MATKEY_OPACITY, opacity);
		print(std::string("Processing material."));

		Rendering::Material_* outMaterial = new Rendering::Material_();
		aiString str;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			//std::cout << "Texture: " << str.C_Str() << std::endl;
			const std::string path = Engine::File::findPath(Engine::File::clipPath(str.C_Str()));
			//std::cout << "T:" << s << std::endl;
			if (!path.empty() && path.find('*') == std::string::npos) outMaterial->setDiffuseMap(Rendering::Texture_::load(path));
		}

		if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
		{
			material->GetTexture(aiTextureType_SPECULAR, 0, &str);
			const std::string path = Engine::File::findPath(Engine::File::clipPath(str.C_Str()));
			if (!path.empty() && path.find('*') == std::string::npos) outMaterial->setSpecularMap(Rendering::Texture_::load(path));
		}

		if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
		{
			material->GetTexture(aiTextureType_EMISSIVE, 0, &str);
			const std::string path = Engine::File::findPath(Engine::File::clipPath(str.C_Str()));
			if (!path.empty() && path.find('*') == std::string::npos)outMaterial->setEmissionMap(Rendering::Texture_::load(path));
		}

		if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
		{
			material->GetTexture(aiTextureType_NORMALS, 0, &str);
			const std::string path = Engine::File::findPath(Engine::File::clipPath(str.C_Str()));
			if (!path.empty() && path.find('*') == std::string::npos)outMaterial->setEmissionMap(Rendering::Texture_::load(path));
		}

		print("Processed material.");
		_recursionLevel--;
		return outMaterial;
	}

	glm::mat4 Model::convert(const aiMatrix4x4 aiMatrix)
	{
		return glm::mat4(
			aiMatrix.a1, aiMatrix.a2, aiMatrix.a3, aiMatrix.a4,
			aiMatrix.b1, aiMatrix.b2, aiMatrix.b3, aiMatrix.b4,
			aiMatrix.c1, aiMatrix.c2, aiMatrix.c3, aiMatrix.c4,
			aiMatrix.d1, aiMatrix.d2, aiMatrix.d3, aiMatrix.d4
		);
	}

	void Model::debug(const bool debug)
	{
		_debug = debug;
	}

	void Model::print(const std::string& message)
	{
		if (_debug)
		{
			std::string prefix;
			for (int i = 0; i < _recursionLevel; i++)
				prefix += "\t";
			std::cout << prefix + message << std::endl;
		}
	}

	std::string Model::removeParentFolders(std::string path)
	{
		while (path.find('\\') != std::string::npos)
		{
			path = path.substr(path.find('\\') + 1, std::string::npos);
			//std::cout << gameStruct.meshName << std::endl;
		}

		return path;
	}

	glm::vec3 Model::_getTranslation(const glm::mat4& matrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(matrix, scale, rotation, translation, skew, perspective);

		return translation;
	}

	glm::quat Model::_getOrientation(const glm::mat4& matrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(matrix, scale, rotation, translation, skew, perspective);

		return rotation;
	}

	glm::vec3 Model::_getScale(const glm::mat4& matrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(matrix, scale, rotation, translation, skew, perspective);

		return scale;
	}

	glm::vec3 Model::_getSkew(const glm::mat4& matrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(matrix, scale, rotation, translation, skew, perspective);

		return skew;
	}

	glm::vec4 Model::_getPerspective(const glm::mat4& matrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(matrix, scale, rotation, translation, skew, perspective);

		return perspective;
	}

	//ShallowMesh* Model::loadModelShallow(const std::string& path)
	//{
	//	std::cout << "Loading model at path " + path << std::endl;
	//	Assimp::Importer import;
	//	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	//	{
	//		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
	//		return nullptr;
	//	}
	//	//this->path = path.substr(0, path.find_last_of('/'));

	//	ShallowMesh* shallow = processNodeShallow(scene->mRootNode, scene);
	//	std::cout << "Loaded model at path " + path << std::endl;
	//	_recursionLevel = 0;
	//	return shallow;
	//}

	//ShallowMesh* Model::processNodeShallow(aiNode* node, const aiScene* scene)
	//{
	//	_recursionLevel++;
	//	print(std::string("Processing node ") + node->mName.C_Str());
	//	ShallowMesh* shallow;// = new GameObject_(node->mName.C_Str(), "");
	//						 // process all the node's meshes (if any)
	//	print(std::string("Has ") + std::to_string(node->mNumMeshes) + " amount of meshes.");
	//	print(std::string("Has ") + std::to_string(node->mNumChildren) + " amount of children.");

	//	bool skip = false;
	//	// then do the same for each of its children
	//	if (node->mNumChildren == 1 && node == scene->mRootNode)
	//	{
	//		shallow = processNodeShallow(node->mChildren[0], scene);
	//		skip = true;
	//	}
	//	else
	//		shallow = new ShallowMesh();

	//	//go->getTransform()->setWorldMatrix4X4(convert(node->mTransformation));

	//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	//	{
	//		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	//		Rendering::Material_* mymat = nullptr;
	//		Rendering::Mesh_* mymesh = nullptr;
	//		const Rendering::RenderQueue queue = processMesh(mesh, scene, mymesh, mymat);

	//		std::cout << "Wrong" << std::endl;
	//		std::cout << "My Mesh: " + std::to_string(mymesh != nullptr) << std::endl;
	//		std::cout << "My Material: " + std::to_string(mymat != nullptr) << std::endl;
	//		shallow->mesh = mymesh;
	//		shallow->material = mymat;
	//		shallow->renderer = new Rendering::Renderer_(mymat, mymesh, queue);
	//	}

	//	if (!skip)
	//		for (unsigned int i = 0; i < node->mNumChildren; i++)
	//		{
	//			ShallowMesh* child = processNodeShallow(node->mChildren[i], scene);
	//			//childGO->getTransform()->setParent(shallow->getTransform(), true);
	//			child->parent = shallow;
	//		}

	//	print(std::string("Processed node ") + node->mName.C_Str());
	//	//std::cout << "Processed node " << node->mName.C_Str() << std::endl;

	//	_recursionLevel--;
	//	return shallow;
	//	return nullptr;
	//}
}
