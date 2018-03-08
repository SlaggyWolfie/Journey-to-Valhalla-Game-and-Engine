#ifndef DESEREALIZER_HPP
#define DESEREALIZER_HPP

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../_vs2015/Scene.hpp"

using namespace std;

struct GameObject_s
{
	string name;
	string meshName;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	int parentID;
	int selfID;

	GameObject_s();
	~GameObject_s() = default;
	GameObject_s(const GameObject_s& other) = default;
	GameObject_s& operator=(const GameObject_s& other) = default;
};
class Deserealizer
{
public:
	vector<GameObject_s> structs;
	Deserealizer();
	void deserializeIntoStructs();
	string oneMesh;
};

#endif // DESEREALIZER_HPP
