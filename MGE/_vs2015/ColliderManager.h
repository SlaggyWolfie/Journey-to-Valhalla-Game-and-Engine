#pragma once
#include "Manager.hpp"
#include <vector>
using namespace Engine;
class collider;

class ColliderManager:public Manager
{
public:
	void initialize() override;
	ColliderManager();
	~ColliderManager();
	std::vector<collider*> CheckCollision(collider* object);
	std::vector<collider*> CheckBoxCollision(collider* object);

	bool CollisionBetween(collider* obj1, collider* obj2);
	glm::vec3 GiveVectorBeetweem(collider* obj1, collider* obj2);
	void addCollider(collider* collider);
	void removeCollider(int i);
	collider* GetColliderByName(std::string GameObjectName);
	collider* getCollider(int index);
	std::vector<collider*> getColliders();
	bool isColliderRegistered(collider* collider);
private:
	std::vector<collider*> allColliders;

};

