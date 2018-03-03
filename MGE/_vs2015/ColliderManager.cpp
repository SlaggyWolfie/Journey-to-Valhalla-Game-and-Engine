#include "ColliderManager.h"



void ColliderManager::initialize()
{
}

ColliderManager::ColliderManager()
{

}


ColliderManager::~ColliderManager()
{

}

std::vector<collider> ColliderManager::CheckCollision(collider object)
{
	std::vector<collider> collideList;
	for (int i = allColliders.size(); i > 0; i--)
	{
		// get box closest point to object.GetPos() center by clamping
		int distance = glm::sqrt((object.GetPos().x - allColliders[i].GetPos().x) * (object.GetPos().x - allColliders[i].GetPos().x) +
			(object.GetPos().y - allColliders[i].GetPos().y) * (object.GetPos().y - allColliders[i].GetPos().y) +
			(object.GetPos().z - allColliders[i].GetPos().z) * (object.GetPos().z - allColliders[i].GetPos().z));
		if (distance < (object.GetRadius() + allColliders[i].GetRadius()))
		{
			collideList.push_back(allColliders[i]);
		}
	}
	return collideList;
}
