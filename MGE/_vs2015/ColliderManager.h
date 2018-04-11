#pragma once
#include "Manager.hpp"
#include <vector>
class collider;
namespace Engine { namespace Core { class GameObject_; } }
class ColliderManager:public Engine::Manager
{
public:
	void initialize() override;
	ColliderManager();
	~ColliderManager();
	std::vector<collider*> CheckCollision(collider* object);
	std::vector<collider*> CheckBoxCollision(collider* object);
	bool CheckBoxCollisionBetween(collider * object, collider * object2);
	bool CheckOBBCollisionBetween(collider * object, collider * object2);

	//glm::vec3 GetCollisionNormal(GameO)

	void CheckOBB(collider* obj);

	bool CollisionBetween(collider* obj1, collider* obj2);
	glm::vec3 GiveVectorBeetweem(collider* obj1, collider* obj2);
	glm::vec3 GiveVectorBeetweenObjects(Engine::Core::GameObject_* obj1, Engine::Core::GameObject_* obj2);
	void addCollider(collider* collider);
	void removeCollider(int i);
	float PlayerY;
	float CrateY = 1.0f;

	collider* GetColliderByName(std::string GameObjectName);
	collider* getCollider(int index);
	std::vector<collider*> getColliders();
	bool isColliderRegistered(collider* collider);
	void reset() override;
private:
	std::vector<collider*> allColliders;

};

