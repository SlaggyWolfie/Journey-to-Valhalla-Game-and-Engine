#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <glm.hpp>
#include <../_vs2015/Scene.h>
#include <../_vs2015/GameObject_.hpp>
#include <../_vs2015/Transform.hpp>
#include "Deserealizer.hpp"
#include "Game.hpp"

using namespace nlohmann;
using namespace std;
using namespace Engine::Core;

Deserealizer::Deserealizer() {
	gms = vector<GameObject_*>();
	ConstructGameObj();
}


void Deserealizer::ConstructGameObj()
{

	struct GameObject_s {
		string name_;
		string meshName;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		int parentID;
		int selfID;
		
	};

	json j;

	vector <GameObject_s> objList;
	ifstream i("test.json");
	ostringstream tmp;
	tmp << i.rdbuf();
	string s = tmp.str();
	string objs;
	json ju;
	ju = json::parse(s);
	//cout << ju.at(2).at("GameObject").at("transform")["rotation"]["x"];
	cout << ju.at(2).at("GameObject").at("name");



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

		objList.push_back(gameObj);
	}

	Scene parsedScene;

	for (int i = 0; i < objList.size(); i++)
	{
		GameObject_* obj = new GameObject_(objList[i].name_, "", objList[i].position);
		//obj->getTransform()->setRotation(/*convert to quaternion here*/);
		obj->getTransform()->setPosition(objList[i].position);
		obj->getTransform()->setRotation(objList[i].rotation);
		obj->getTransform()->setScale(objList[i].scale);
		obj->setName(objList[i].name_);

		obj->setName(objList[i].meshName);
		gms.push_back(obj);
	}


}