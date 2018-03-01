#ifndef DESEREALIZER_HPP
#define DESEREALIZER_HPP

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "../_vs2015/Scene.hpp"

using namespace std;

struct GameObject_s 
{
	string name_;
	string meshName;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	int parentID;
	int selfID;

};
class Deserealizer
{
public:
	vector<GameObject_s> structs;
	Deserealizer();
	void ConstructGameObj();
	string oneMesh;
};

#endif // DESEREALIZER_HPP
