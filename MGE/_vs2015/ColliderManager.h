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
	void addCollider(collider* collider);
	void removeCollider(collider* collider);
	void removeCollider(int index);
	collider* getCollider(int index);
	bool isColliderRegistered(collider* collider);
private:
	std::vector<collider*> allColliders;

};

