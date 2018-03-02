#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <glm.hpp>
<<<<<<< HEAD
//#include <../_vs2015/Scene.hpp>
=======
>>>>>>> 1acb3faf321b6ae6a5bf9d5770ae4edbfd6ff9be
#include <../_vs2015/GameObject_.hpp>
#include <../_vs2015/Transform.hpp>
#include "Deserealizer.hpp"
#include "Game.hpp"
//#include "../_vs2015/Scene.hpp"

using namespace nlohmann;
using namespace std;
using namespace Engine::Core;

Deserealizer::Deserealizer() {

	structs = vector<GameObject_s>();
	ConstructGameObj();
}


void Deserealizer::ConstructGameObj()
{

	

	json j;

	ifstream i("test.json");
	ostringstream tmp;
	tmp << i.rdbuf();
	string s = tmp.str();
	string objs;
	json ju;
	ju = json::parse(s);
	//cout << ju.at(2).at("GameObject").at("transform")["rotation"]["x"];



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
	int g = 0;
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
	for (int j = 0; j < static_cast<int>(ju.size()); j++)
	{
		GameObject_s gameObj;
		string s = ju.at(j).at("GameObject").at("name");
		//cout << s;
		gameObj.name_ = s;
		std::cout << s;
		gameObj.position.x= ju.at(j).at("GameObject").at("transform")["position"]["x"];
		gameObj.position.y = ju.at(j).at("GameObject").at("transform")["position"]["y"];
		gameObj.position.z = ju.at(j).at("GameObject").at("transform")["position"]["z"];

		gameObj.rotation.x = ju.at(j).at("GameObject").at("transform")["rotation"]["x"];
		gameObj.rotation.y = ju.at(j).at("GameObject").at("transform")["rotation"]["y"];
		gameObj.rotation.z = ju.at(j).at("GameObject").at("transform")["rotation"]["z"];

		gameObj.scale.x = ju.at(j).at("GameObject").at("transform")["scale"]["x"];
		gameObj.scale.y = ju.at(j).at("GameObject").at("transform")["scale"]["y"];
		gameObj.scale.z = ju.at(j).at("GameObject").at("transform")["scale"]["z"];
		string n= ju.at(j).at("GameObject").at("meshString");
		gameObj.meshName = n;
		if (j == 0)
			oneMesh = n;

		gameObj.parentID = ju.at(j).at("GameObject").at("parentID");
		gameObj.selfID=ju.at(j).at("GameObject").at("selfID");

		structs.push_back(gameObj);

	}


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

	for (int i = 0; i < structs.size(); i++)
	{
		Engine::Core::GameObject_* obj = new Engine::Core::GameObject_(structs[i].name_, "", structs[i].position);
		//obj->getTransform()->setRotation(/*convert to quaternion here*/);
		obj->getTransform()->setPosition(structs[i].position);
		obj->getTransform()->setRotation(structs[i].rotation);
		obj->getTransform()->setScale(structs[i].scale);
		obj->setName(structs[i].name_);

	}


}