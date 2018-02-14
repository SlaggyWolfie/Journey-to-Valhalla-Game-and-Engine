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
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		
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
		objList.push_back(gameObj);
	}


	/*for (int j = 0; j < GameObjString.size(); j++)
	{
	string str = GameObjString[j];
	string temp;
	string name;
	json jeson;
	for (int k = 0; k < str.length(); k++)
	{
	if (str[k] != '}')
	{
	temp += str[k];
	if()
	}
	else
	{


	}
	}
	}*/


	string str;
	getline(cin, str);


}