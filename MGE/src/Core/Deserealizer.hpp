#ifndef DESEREALIZER_HPP
#define DESEREALIZER_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
	int lightType;
	float lightIntencity;
	glm::vec4 lightColor;
	glm::vec3 colliderSize;
	glm::vec3 colliderCenter;


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
	void deserializeIntoStructs(const string& fileName);
	string oneMesh;
};

#endif // DESEREALIZER_HPP
