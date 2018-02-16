#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
#include <vector>
#include <glm.hpp>

using namespace nlohmann;
using namespace std;


void ConstructGameObj()
{

	struct GameObject_ {
		string name_;
		string meshName;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		int parentID;
		int selfID;
		
	};

	json j;

	vector <GameObject_> objList;
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
	for (int j = 0; j < ju.size(); j++)
	{
		GameObject_ gameObj;
		string s = ju.at(2).at("GameObject").at("name");
		//cout << s;
		gameObj.name_ = s;

		gameObj.position.x= ju.at(2).at("GameObject").at("transform")["position"]["x"];
		gameObj.position.y = ju.at(2).at("GameObject").at("transform")["position"]["y"];
		gameObj.position.z = ju.at(2).at("GameObject").at("transform")["position"]["z"];

		gameObj.rotation.x = ju.at(2).at("GameObject").at("transform")["rotation"]["x"];
		gameObj.rotation.y = ju.at(2).at("GameObject").at("transform")["rotation"]["y"];
		gameObj.rotation.z = ju.at(2).at("GameObject").at("transform")["rotation"]["z"];

		gameObj.scale.x = ju.at(2).at("GameObject").at("transform")["scale"]["x"];
		gameObj.scale.y = ju.at(2).at("GameObject").at("transform")["scale"]["y"];
		gameObj.scale.z = ju.at(2).at("GameObject").at("transform")["scale"]["z"];
		string n= ju.at(2).at("GameObject").at("meshString");
		gameObj.meshName = n;

		gameObj.parentID = ju.at(2).at("GameObject").at("parentID");
		gameObj.selfID=ju.at(2).at("GameObject").at("selfID");

		objList.push_back(gameObj);
	}


}