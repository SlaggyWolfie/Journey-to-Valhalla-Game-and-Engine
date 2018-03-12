#include "ColliderManager.h"
#include "collider.h"
#include "GameObject_.hpp"
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
		}
	}
		return collideList;
}
std::vector<collider*> ColliderManager::CheckBoxCollision(collider * object)
{
	std::vector<collider*> collideList;
	//std::cout << allColliders.size() << std::endl;
	for (int i = 0; i <allColliders.size(); i++)
	{
		if (allColliders[i]->getGameObject()->getName() != object->getGameObject()->getName())
		{
			//std::cout<<allColliders.size() <<"  " << allColliders[i]->getGameObject()->getName() <<"names are different"<< object->getGameObject()->getName() << std::endl;
			//std::cout << " sir!" << std::endl;
			/*std::cout << ((object->point1.x <= allColliders[i]->point2.x && object->point2.x >= allColliders[i]->point1.x) &&
				(object->point3.z <= allColliders[i]->point4.z && object->point4.z >= allColliders[i]->point3.z) &&
				(object->point5.y <= allColliders[i]->point6.y && object->point6.y >= allColliders[i]->point5.y)) << std::endl;*/
			bool yCheck = (object->point5.y <= allColliders[i]->point6.y && object->point6.y >= allColliders[i]->point5.y);
			bool zCheck = (object->point3.z <= allColliders[i]->point4.z && object->point4.z >= allColliders[i]->point3.z);
			bool xCheck = (object->point1.x <= allColliders[i]->point2.x && object->point2.x >= allColliders[i]->point1.x);
			if (xCheck&&
				zCheck &&
				yCheck)
			{
				std::cout << "its collision, sir!" << std::endl;
				collideList.push_back(allColliders[i]);
			}
		}
	}
		return collideList;
}

bool ColliderManager::CheckBoxCollisionBetween(collider * object, collider * object2)
{
	if ((!object->Enabled() || !object2->Enabled())) return false;

			//std::cout<<allColliders.size() <<"  " << allColliders[i]->getGameObject()->getName() <<"names are different"<< object->getGameObject()->getName() << std::endl;
			//std::cout << " sir!" << std::endl;

			/*std::cout << ((object->point1.x <= allColliders[i]->point2.x && object->point2.x >= allColliders[i]->point1.x) &&
			(object->point3.z <= allColliders[i]->point4.z && object->point4.z >= allColliders[i]->point3.z) &&
			(object->point5.y <= allColliders[i]->point6.y && object->point6.y >= allColliders[i]->point5.y)) << std::endl;*/
			bool yCheck = (object->point5.y <= object2->point6.y && object->point6.y >= object2->point5.y);
			bool zCheck = (object->point3.z <= object2->point4.z && object->point4.z >= object2->point3.z);
			bool xCheck = (object->point1.x <= object2->point2.x && object->point2.x >= object2->point1.x);
			std::cout << "X " << xCheck << " Z " << zCheck << " Y " << yCheck << std::endl;
			if (xCheck &&
				zCheck &&
				yCheck)
			{
				std::cout << "!!!!" << std::endl;
				return true;
			}
			else return false;
}


glm::vec3 ColliderManager::GiveVectorBeetweem(collider * obj1, collider * obj2)
{
	glm::vec3 l = obj1->getGameObject()->getTransform()->getPosition() - obj2->getGameObject()->getTransform()->getPosition();
	return l;
}

glm::vec3 ColliderManager::GiveVectorBeetweenObjects(Engine::Core::GameObject_ * obj1, Engine::Core::GameObject_ * obj2)
{
	glm::vec3 l = obj1->getTransform()->getPosition() - obj2->getTransform()->getPosition();
	return l;
}

//C++ <3
bool ColliderManager::CollisionBetween(collider * obj1, collider * obj2)
{
	if ((!obj1->Enabled() || !obj2->Enabled())) return false;
	int distance = glm::sqrt((obj1->GetPos().x - obj2->GetPos().x) * (obj1->GetPos().x - obj2->GetPos().x) +
		(obj1->GetPos().y - obj2->GetPos().y) * (obj1->GetPos().y - obj2->GetPos().y) +
		(obj1->GetPos().z - obj2->GetPos().z) * (obj1->GetPos().z - obj2->GetPos().z));
	//std::cout << distance << std::endl;
	return distance < (obj1->GetRadius() + obj2->GetRadius());
}

void ColliderManager::addCollider(collider* collider)
{
	allColliders.push_back(collider);
}

//void ColliderManager::removeCollider(collider* collider)
//{
//	allColliders.erase(std::remove(allColliders.begin(), allColliders.end(), collider), allColliders.end());
//}

void ColliderManager::removeCollider(int index)
{
	allColliders.erase(allColliders.begin() + index);
}

collider* ColliderManager::GetColliderByName(std::string GameObjectName)
{
	
	for (int i = 0; i < allColliders.size(); i++)
	{
		//std::cout << allColliders[i]->getGameObject()->getName() << std::endl;
		if (GameObjectName==allColliders[i]->getGameObject()->getName())
			return allColliders[i];
	}
	return 0;
}

collider* ColliderManager::getCollider(int index)
{
	return allColliders[index];
}

std::vector<collider*> ColliderManager::getColliders()
{
	return allColliders;
}

bool ColliderManager::isColliderRegistered(collider* collider)
{
	return std::find(allColliders.begin(), allColliders.end(), collider) != allColliders.end();
}
