#include "ColliderManager.h"
#include "collider.h"
#include <vector>
#include <iostream>
#include "glm.hpp"


void ColliderManager::initialize()
{
}

ColliderManager::ColliderManager() : allColliders(std::vector<collider*>())
{
	
}


ColliderManager::~ColliderManager()
{

}

std::vector<collider*> ColliderManager::CheckCollision(collider* object)
{
	std::vector<collider*> collideList;
	//std::cout << allColliders.size() << std::endl;
	for (int i = allColliders.size()-1; i >= 0; i--)
	{
		if (object != allColliders[i])
		{
			//this line gives error because of list of pointers(?)
			//std::cout << std::to_string(allColliders[i]->GetPos().y) + " pos y" << std::endl;
			//// get box closest point to object->GetPos() center by clamping
			int distance = glm::sqrt((object->GetPos().x - allColliders[i]->GetPos().x) * (object->GetPos().x - allColliders[i]->GetPos().x) +
				(object->GetPos().y - allColliders[i]->GetPos().y) * (object->GetPos().y - allColliders[i]->GetPos().y) +
				(object->GetPos().z - allColliders[i]->GetPos().z) * (object->GetPos().z - allColliders[i]->GetPos().z));

			if (distance < (object->GetRadius() + allColliders[i]->GetRadius()))
			{
				std::cout << "Collision!" << std::endl;
				collideList.push_back(allColliders[i]);
				//std::cout << collideList.size() << std::endl;
			}
			else
			{
				//std::cout << collideList.size() << std::endl;

				/*std::cout << collideList.size() << std::endl;
				if (!collideList.empty())
				{
					auto element = find(collideList.begin(), collideList.end(), allColliders[i]) - collideList.begin();
					std::cout << element;

				}*/
			}
		return collideList;
		}
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
