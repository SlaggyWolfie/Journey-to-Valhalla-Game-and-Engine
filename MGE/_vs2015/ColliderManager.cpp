#include "ColliderManager.h"
#include "collider.h"
#include "GameObject_.hpp"
#include <iostream>
#include "glm.hpp"
#include <SFML/Window.hpp>


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
	for (int i = allColliders.size() - 1; i >= 0; i--)
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
				//std::cout << "Collision!" << std::endl;
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
	for (unsigned i = 0; i < allColliders.size(); i++)
	{
		if (allColliders[i]->getGameObject()->getName() != object->getGameObject()->getName())
		{
			//make obb
			glm::vec3 oneCenter = object->getGameObject()->getTransform()->getPosition();


			//if (allColliders[i]->getGameObject()->getName() == "Main character 1.2 updated"&&object->getGameObject()->getName() == "Pressure plate 1")
			//{	/*
			/*std::cout<<allColliders.size() <<"  " << allColliders[i]->getGameObject()->getName() <<"names are different"<< object->getGameObject()->getName() << std::endl;
			std::cout << " sir!" << std::endl;
			std::cout << object->point1 << "   " << allColliders[i]->point1 << std::endl;*/
			/*std::cout << ((object->point1.x <= allColliders[i]->point2.x && object->point2.x >= allColliders[i]->point1.x) &&
				(object->point3.z <= allColliders[i]->point4.z && object->point4.z >= allColliders[i]->point3.z) &&
				(object->point5.y <= allColliders[i]->point6.y && object->point6.y >= allColliders[i]->point5.y)) << std::endl;*/

			bool yCheck = (object->point5.y <= allColliders[i]->point6.y && object->point6.y >= allColliders[i]->point5.y);
			bool zCheck = (object->point3.z <= allColliders[i]->point4.z && object->point4.z >= allColliders[i]->point3.z);
			bool xCheck = (object->point1.x <= allColliders[i]->point2.x && object->point2.x >= allColliders[i]->point1.x);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
			{
				//std::cout << "checking collisions" << std::endl;
				//Main character 1.2 updated
				if (allColliders[i]->getGameObject()->getName() == "Main character 1.2 updated")
				{
					std::cout << allColliders[i]->getGameObject()->getName();

					std::cout << "Xobj min " << object->point1.x << " Xother max " << allColliders[i]->point2.x << " " <<
						object->point2.x << " " << allColliders[i]->point1.x << std::endl;
					std::cout << "Zobj min " << object->point3.z << " Zother max " << allColliders[i]->point4.z << " " <<
						object->point4.z << " " << allColliders[i]->point3.z << std::endl;
					/*std::cout << object->point1 << ":x: " << allColliders[i]->point2 << std::endl;
					std::cout << object->point3 << ":y: " << allColliders[i]->point3 << std::endl;
					std::cout << object->point5 << ":z: " << allColliders[i]->point5 << std::endl;*/
				}

			}


			if (xCheck &&
				zCheck &&
				yCheck)
			{
				if (CheckOBBCollisionBetween(object, allColliders[i]))
				{

					std::cout << "its collision, sir!" << std::endl;
					collideList.push_back(allColliders[i]);
				}
			}
			//}
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

	return xCheck && zCheck && yCheck;
}

bool ColliderManager::CheckOBBCollisionBetween(collider * object, collider * object2)
{
	glm::vec3 object1center = object->getGameObject()->getTransform()->getPosition();
	glm::mat4 object1transform = glm::scale(object->getGameObject()->getTransform()->getMatrix4X4(), object->halfSize); // scaling for halfsize
	glm::vec3 object2center = object2->getGameObject()->getTransform()->getPosition();
	glm::mat4 object2transform = glm::scale(object2->getGameObject()->getTransform()->getMatrix4X4(), object2->halfSize);

	for (int a = 0; a < 3; a++) 
	{
		glm::vec3 line = glm::vec3(object1transform[a]); // one axis to project on
		float differenceProjection = glm::abs(glm::dot(line, object2center) - glm::dot(line, object1center)); // center distance
		float object1projection = glm::abs(glm::dot(line, glm::vec3(object1transform[0])))
			+ glm::abs(glm::dot(line, glm::vec3(object1transform[1])))
			+ glm::abs(glm::dot(line, glm::vec3(object1transform[2])));
		float object2projection = glm::abs(glm::dot(line, glm::vec3(object2transform[0])))
			+ glm::abs(glm::dot(line, glm::vec3(object2transform[1])))
			+ glm::abs(glm::dot(line, glm::vec3(object2transform[2])));
		float penetration = (object1projection + object2projection) - differenceProjection;
		if (penetration <= 0) 
		{ // no overlap
			return false;
		}
	}

	for (int b = 0; b < 3; b++) 
	{
		glm::vec3 line = glm::vec3(object2transform[b]); // other axis to project on
		float tl = glm::abs(glm::dot(line, object2center) - glm::dot(line, object1center)); // center distance
		float ra = glm::abs(glm::dot(line, glm::vec3(object1transform[0])))
			+ glm::abs(glm::dot(line, glm::vec3(object1transform[1])))
			+ glm::abs(glm::dot(line, glm::vec3(object1transform[2])));
		float rb = glm::abs(glm::dot(line, glm::vec3(object2transform[0])))
			+ glm::abs(glm::dot(line, glm::vec3(object2transform[1])))
			+ glm::abs(glm::dot(line, glm::vec3(object2transform[2])));
		float penetration = (ra + rb) - tl;
		if (penetration <= 0) 
		{ // no overlap
			return false;
		}
	}

	for (int a = 0; a < 3; a++) 
	{
		glm::vec3 aAxis = glm::vec3(object1transform[a]);
		for (int b = 0; b < 3; b++) {
			glm::vec3 bAxis = glm::vec3(object2transform[b]);
			if (aAxis != bAxis) {
				glm::vec3 l = glm::cross(aAxis, bAxis); // has flaw when axis are same, result in (0,0,0), solved by if
				float tl = std::abs(glm::dot(l, object2center) - glm::dot(l, object1center)); // center distance
				float ra = std::abs(glm::dot(l, glm::vec3(object1transform[0]))) + std::abs(glm::dot(l, glm::vec3(object1transform[1]))) + std::abs(glm::dot(l, glm::vec3(object1transform[2])));
				float rb = std::abs(glm::dot(l, glm::vec3(object2transform[0]))) + std::abs(glm::dot(l, glm::vec3(object2transform[1]))) + std::abs(glm::dot(l, glm::vec3(object2transform[2])));
				float penetration = (ra + rb) - tl;
				if (penetration <= 0) 
				{ // no overlap
					return false;
				}
			}
		}
	}

	return true;
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

	for (unsigned i = 0; i < allColliders.size(); i++)
	{
		//std::cout << allColliders[i]->getGameObject()->getName() << std::endl;
		if (GameObjectName == allColliders[i]->getGameObject()->getName())
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

void ColliderManager::reset()
{
	allColliders.clear();
}
