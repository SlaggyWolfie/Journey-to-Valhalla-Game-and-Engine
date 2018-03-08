#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <../_vs2015/GameObject_.hpp>
#include <../_vs2015/Transform.hpp>
#include "Deserealizer.hpp"
#include "Game.hpp"

using namespace nlohmann;
using namespace std;
using namespace Engine::Core;

GameObject_s::GameObject_s() :
	name("defaultName"), meshName("defaultMEshName"), position(glm::vec3(0)), rotation(glm::vec3(0)), scale(glm::vec3(1)), parentID(0),
	selfID(0)
{
}

Deserealizer::Deserealizer() {

	structs = vector<GameObject_s>();
	//deserializeIntoStructs();
}


void Deserealizer::deserializeIntoStructs()
{
	ifstream sceneFile("test.json");
	ostringstream temporaryBuffer;
	temporaryBuffer << sceneFile.rdbuf();
	const string entireSceneString = temporaryBuffer.str();
	json jsonScene = json::parse(entireSceneString);

	for (int j = 0; j < static_cast<int>(jsonScene.size()); j++)
	{
		GameObject_s gameObjectStruct;
		//GameObject_s gameObjectStruct = GameObject_s();
		gameObjectStruct.name = jsonScene.at(j).at("GameObject").at("name").get<string>();
		//std::cout << "Desearlizing game object " + gameObjectStruct.name << std::endl;
		gameObjectStruct.position.x = jsonScene.at(j).at("GameObject").at("transform")["position"]["x"];
		gameObjectStruct.position.y = jsonScene.at(j).at("GameObject").at("transform")["position"]["y"];
		gameObjectStruct.position.z = jsonScene.at(j).at("GameObject").at("transform")["position"]["z"];

		gameObjectStruct.rotation.x = jsonScene.at(j).at("GameObject").at("transform")["rotation"]["x"];
		gameObjectStruct.rotation.y = jsonScene.at(j).at("GameObject").at("transform")["rotation"]["y"];
		gameObjectStruct.rotation.z = jsonScene.at(j).at("GameObject").at("transform")["rotation"]["z"];

		gameObjectStruct.scale.x = jsonScene.at(j).at("GameObject").at("transform")["scale"]["x"];
		gameObjectStruct.scale.y = jsonScene.at(j).at("GameObject").at("transform")["scale"]["y"];
		gameObjectStruct.scale.z = jsonScene.at(j).at("GameObject").at("transform")["scale"]["z"];
		const string n = jsonScene.at(j).at("GameObject").at("meshString");
		gameObjectStruct.meshName = n;
		if (j == 0)
			oneMesh = n;

		gameObjectStruct.parentID = jsonScene.at(j).at("GameObject").at("parentID");
		gameObjectStruct.selfID = jsonScene.at(j).at("GameObject").at("selfID");

		structs.push_back(gameObjectStruct);
	}


	//cout << ju.is_array();
	//auto rex = json::parse(s);
	//cout << rex;
	//cout<<j.is_array();
	//j.parse(tmp.str());
	//string name = ju['name'];
	//cout << ju.get<string>(); 
	//for (auto& element : j) {
	//	std::cout << element << '\n';
	//}
	//cout << j.begin().key() << endl;
	//int g = 0;
	//for (int j = 0; j < static_cast<int>(ju.size()); j++)
	//{
	//	GameObject_s gameObj;
	//	string s = ju.at(2).at("GameObject").at("name");
	//	//cout << s;
	//	gameObj.name_ = s;

	//	gameObj.position.x= ju.at(2).at("GameObject").at("transform")["position"]["x"];
	//	gameObj.position.y = ju.at(2).at("GameObject").at("transform")["position"]["y"];
	//	gameObj.position.z = ju.at(2).at("GameObject").at("transform")["position"]["z"];

	//	gameObj.rotation.x = ju.at(2).at("GameObject").at("transform")["rotation"]["x"];
	//	gameObj.rotation.y = ju.at(2).at("GameObject").at("transform")["rotation"]["y"];
	//	gameObj.rotation.z = ju.at(2).at("GameObject").at("transform")["rotation"]["z"];

	//	gameObj.scale.x = ju.at(2).at("GameObject").at("transform")["scale"]["x"];
	//	gameObj.scale.y = ju.at(2).at("GameObject").at("transform")["scale"]["y"];
	//	gameObj.scale.z = ju.at(2).at("GameObject").at("transform")["scale"]["z"];
	//	string n= ju.at(2).at("GameObject").at("meshString");
	//	gameObj.meshName = n;

	//	gameObj.parentID = ju.at(2).at("GameObject").at("parentID");
	//	gameObj.selfID=ju.at(2).at("GameObject").at("selfID");

	//	structs.push_back(gameObj);
	//}


	//for (int i = 0; i < structs.size(); i++)
	//{
	//	GameObject_* obj = new GameObject_(structs[i].name_, "", structs[i].position);
	//	//obj->getTransform()->setRotation(/*convert to quaternion here*/);
	//	obj->getTransform()->setPosition(structs[i].position);
	//	obj->getTransform()->setRotation(structs[i].rotation);
	//	obj->getTransform()->setScale(structs[i].scale);
	//	obj->setName(structs[i].name_);

	//	obj->setName(structs[i].meshName);
	//	//gms.push_back(obj);
	//}

	//Engine::Scene* parsedScene = nullptr;

	//for (int i = 0; i < structs.size(); i++)
	//{
	//	Engine::Core::GameObject_* obj = new Engine::Core::GameObject_(structs[i].name, "", structs[i].position);
	//	//obj->getTransform()->setRotation(/*convert to quaternion here*/);
	//	obj->getTransform()->setPosition(structs[i].position);
	//	obj->getTransform()->setRotation(structs[i].rotation);
	//	obj->getTransform()->setScale(structs[i].scale);
	//	obj->setName(structs[i].name);
	//}


}