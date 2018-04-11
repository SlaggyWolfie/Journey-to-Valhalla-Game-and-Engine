#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <../_vs2015/GameObject_.hpp>
#include <../_vs2015/Transform.hpp>
#include "Light_.hpp"
#include "Mesh_.hpp"
#include "collider.h"
#include "Deserializer2.hpp"

using namespace nlohmann;
using namespace std;
using namespace Engine::Core;

GameStruct::GameStruct() : transform(nullptr)
{
}

Component* ComponentStruct::makeObject()
{
	return nullptr;
}

void TransformStruct::deserialize(nlohmann::json& jsonObject)
{
	position = Deserializer2::jsonToVec3(jsonObject["position"]);
	scale = Deserializer2::jsonToVec3(jsonObject["scale"]);
	rotation = Deserializer2::jsonToQuat(jsonObject["rotation"]);

	parentID = jsonObject["parentID"];
	selfID = jsonObject["selfID"];
}

Engine::Core::Component* TransformStruct::makeObject()
{
	return nullptr;

	Transform* transform = new Transform();
	transform->setLocalPosition(position);
	transform->setLocalRotation(rotation);
	transform->setLocalScale(scale);

	return transform;
}

void LightStruct::deserialize(nlohmann::json& jsonObject)
{
	intensity = jsonObject["intensity"];
	type = jsonObject["type"];

	color = Deserializer2::jsonToVec4(jsonObject["color"]);
}

Engine::Core::Component* LightStruct::makeObject()
{
	auto light = new Engine::Rendering::Light_();
	light->setLightIntensity(intensity);
	light->setColor(color);
	light->setLightType(static_cast<Engine::Rendering::LightType>(type));

	return light;
}

void BoxColliderStruct::deserialize(nlohmann::json& jsonObject)
{
	center = Deserializer2::jsonToVec3(jsonObject["center"]);
	size = Deserializer2::jsonToVec3(jsonObject["size"]);
	isTrigger = jsonObject["isTrigger"].get<bool>();
}

Engine::Core::Component* BoxColliderStruct::makeObject()
{
	collider* boxCollider = new collider();
	boxCollider->SetBoxSize(-size.x, size.y, size.z);
	center.x *= -1;
	boxCollider->SetCenterOffset(center);
	boxCollider->SetTrigger(isTrigger);

	return boxCollider;
}

void MeshStruct::deserialize(nlohmann::json& jsonObject)
{
	path = jsonObject["meshPath"].get<string>();
}

Engine::Core::Component* MeshStruct::makeObject()
{
	return nullptr;
}

GameStruct::~GameStruct()
{
	delete transform;
	for (ComponentStruct* component : components)
		delete component;

	components.clear();
}

void GameStruct::deserialize(nlohmann::json& jsonObject)
{
	name = jsonObject["name"].get<string>();
	std::cout << "Deserealizing game object " + name << std::endl;

	tag = jsonObject["tag"].get<string>();
	active = jsonObject["active"].get<bool>();

	transform = new TransformStruct;
	transform->deserialize(jsonObject["transform"]);

	json jsonComponents = jsonObject["components"];
	for (int j = 0; j < static_cast<int>(jsonComponents.size()); j++)
	{
		json jsonComponent = jsonComponents[j];
		json jsonType = jsonComponent["typeID"];
		const int type = jsonType.get<int>();
		switch (type)
		{
		case 0:
		case 1:
		default:
		{
			std::cout << "Something has gone terribly wrong. Deserialization type: "
				+ std::to_string(type) << std::endl;
			break;
		}
		case 2:
		{
			BoxColliderStruct* boxCollider = new BoxColliderStruct;
			boxCollider->deserialize(jsonComponent);
			components.push_back(boxCollider);
			break;
		}
		case 3:
		{
			LightStruct* light = new LightStruct;
			light->deserialize(jsonComponent);
			components.push_back(light);
			break;
		}
		case 4:
		{
			MeshStruct* mesh = new MeshStruct;
			mesh->deserialize(jsonComponent);
			hasMesh = true;
			components.push_back(mesh);
			break;
		}
		}
	}
}

Deserializer2::~Deserializer2()
{
	for (GameStruct* gameStruct : gameStructs)
		delete gameStruct;

	gameStructs.clear();
}

std::vector<GameStruct*> Deserializer2::deserializeIntoStructs(const string& fileName)
{
	ifstream sceneFile(fileName);

	//ifstream sceneFile("test.json");
	ostringstream temporaryBuffer;
	temporaryBuffer << sceneFile.rdbuf();
	const string entireSceneString = temporaryBuffer.str();
	json jsonScene = json::parse(entireSceneString);

	//for (auto & j : jsonScene)
	//{
	//	GameStruct* gameObjectStruct = new GameStruct();
	//	json jsonGameObject = j["GameObject"];
	//	gameObjectStruct->deserialize(jsonGameObject);
	//	gameStructs.push_back(gameObjectStruct);
	//}

	for (int j = 0; j < static_cast<int>(jsonScene.size()); j++)
	{
		GameStruct* gameObjectStruct = new GameStruct();
		json jsonGameObject = jsonScene[j]["GameObject"];
		gameObjectStruct->deserialize(jsonGameObject);
		gameStructs.push_back(gameObjectStruct);
	}

	return gameStructs;
}

glm::vec3 Deserializer2::jsonToVec3(nlohmann::json& jsonObject)
{
	glm::vec3 vector;
	vector.x = jsonObject["x"];
	vector.y = jsonObject["y"];
	vector.z = jsonObject["z"];
	return vector;
}

glm::vec4 Deserializer2::jsonToVec4(nlohmann::json& jsonObject)
{
	glm::vec4 vector;
	vector.x = jsonObject["x"];
	vector.y = jsonObject["y"];
	vector.z = jsonObject["z"];
	vector.w = jsonObject["w"];
	return vector;
}

glm::quat Deserializer2::jsonToQuat(nlohmann::json& jsonObject)
{
	glm::quat quaternion;
	quaternion.x = jsonObject["x"];
	quaternion.y = jsonObject["y"];
	quaternion.z = jsonObject["z"];
	quaternion.w = jsonObject["w"];
	return quaternion;
}

void Deserializer2::jsonToVec3(glm::vec3& vector, nlohmann::json& jsonObject)
{
	vector.x = jsonObject["x"];
	vector.y = jsonObject["y"];
	vector.z = jsonObject["z"];
}

void Deserializer2::jsonToVec4(glm::vec4& vector, nlohmann::json& jsonObject)
{
	vector.x = jsonObject["x"];
	vector.y = jsonObject["y"];
	vector.z = jsonObject["z"];
	vector.w = jsonObject["w"];
}

void Deserializer2::jsonToQuat(glm::quat& quaternion, nlohmann::json& jsonObject)
{
	quaternion.x = jsonObject["x"];
	quaternion.y = jsonObject["y"];
	quaternion.z = jsonObject["z"];
	quaternion.w = jsonObject["w"];
}
