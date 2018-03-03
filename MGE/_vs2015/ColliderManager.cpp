#include "ColliderManager.h"
#include "collider.h"
#include <vector>


void ColliderManager::initialize()
{
}

ColliderManager::ColliderManager():allColliders( std::vector<collider*>())
{
	
}


ColliderManager::~ColliderManager()
{

}

std::vector<collider*> ColliderManager::CheckCollision(collider* object)
{
	std::vector<collider*> collideList;
	std::cout << allColliders.size() << std::endl;
	for (int i = allColliders.size(); i > 0; i--)
	{
		std::cout << allColliders[i]->GetPos().y << " pos y";
		//// get box closest point to object->GetPos() center by clamping
		/*int distance = glm::sqrt((object->GetPos().x - allColliders[i]->GetPos().x) * (object->GetPos().x - allColliders[i]->GetPos().x) +
			(object->GetPos().y - allColliders[i]->GetPos().y) * (object->GetPos().y - allColliders[i]->GetPos().y) +
			(object->GetPos().z - allColliders[i]->GetPos().z) * (object->GetPos().z - allColliders[i]->GetPos().z));
		std::cout << distance;*/
		/*if (distance < (object->GetRadius() + allColliders[i]->GetRadius()))
		{

			collideList.push_back(allColliders[i]);
		}*/
		//else 
		//{
			//if (!collideList.empty())
			//{
		//		auto element = find(collideList.begin(), collideList.end(), allColliders[i])-collideList.begin();
		//		std::cout << element;
		//	//}
		////}
		return collideList;
	}
}

void ColliderManager::addCollider(collider* collider)
{
	allColliders.push_back(collider);
}

void ColliderManager::removeCollider(collider* collider)
{
	allColliders.erase(std::remove(allColliders.begin(), allColliders.end(), collider), allColliders.end());
}

void ColliderManager::removeCollider(int index)
{
	allColliders.erase(allColliders.begin() + index);
}

collider* ColliderManager::getCollider(int index)
{
	return allColliders[index];
}

bool ColliderManager::isColliderRegistered(collider* collider)
{
	return std::find(allColliders.begin(), allColliders.end(), collider) != allColliders.end();
}
