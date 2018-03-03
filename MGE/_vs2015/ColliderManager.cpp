#include "ColliderManager.h"
#include "collider.h"


void ColliderManager::initialize()
{
}

ColliderManager::ColliderManager()
{

}


ColliderManager::~ColliderManager()
{

}

std::vector<collider*> ColliderManager::CheckCollision(collider* object)
{
	std::vector<collider*> collideList;
	for (int i = allColliders.size(); i > 0; i--)
	{
		// get box closest point to object->GetPos() center by clamping
		int distance = glm::sqrt((object->GetPos().x - allColliders[i]->GetPos().x) * (object->GetPos().x - allColliders[i]->GetPos().x) +
			(object->GetPos().y - allColliders[i]->GetPos().y) * (object->GetPos().y - allColliders[i]->GetPos().y) +
			(object->GetPos().z - allColliders[i]->GetPos().z) * (object->GetPos().z - allColliders[i]->GetPos().z));
		if (distance < (object->GetRadius() + allColliders[i]->GetRadius()))
		{
			collideList.push_back(allColliders[i]);
		}
	}
	return collideList;
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
