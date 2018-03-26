#include "collider.h"
#include"ServiceLocator.hpp"
#include "ColliderManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/Game.hpp"
#include "Time.hpp"
#include "PlayerBaseComponent.h"
#include "Camera_.hpp"
using namespace Engine;
collider::collider() : _colliderManager(nullptr), _t(nullptr)
{
}

collider::~collider()
{
}

collider::collider(const collider& other) :
	_colliderManager(other._colliderManager),
	_pos(other._pos), _rot(other._rot), _t(other._t),
	_type(other._type), _width(other._width),
	_height(other._height), _radius(other._radius)
{
}

collider& collider::operator=(const collider& other)
{
	_colliderManager = other._colliderManager;
	_pos = other._pos;
	_rot = other._rot;
	_t = other._t;
	_type = other._type;
	_width = other._width;
	_height = other._height;
	_radius = other._radius;
	return *this;
}

void collider::SetType(int t)
{
	_type = sphere;
}
void collider::SetBoxSize(float w, float h, float l)
{
	_height = h;
	_width = w;
	_length = l;
}
void collider::SetSphereRadius(float r)
{
	_radius = r;
}

void collider::fixedUpdate()
{
}
void collider::PushBackObj(std::string n, std::string n1)
{
	//std::cout << n << " " << n1 << std::endl;
	ColliderManager* c = Engine::ServiceLocator::instance()->getService<ColliderManager>();
	GameObject_* obj1 = c->
		GetColliderByName(n)->getGameObject();
	GameObject_* obj2 = c->
		GetColliderByName(n1)->getGameObject();
	glm::vec3 delta = c->GiveVectorBeetweem(c->GetColliderByName(n), c->GetColliderByName(n1));
	/*std::cout << delta << std::endl;*/
	obj1->getTransform()->setPosition(glm::lerp(obj1->getTransform()->getPosition(),
		obj1->getComponent<PlayerBaseComponent>()->lastPos + delta * glm::vec3(0.01f), 0.9f));

}
bool collider::Enabled()
{
	return _enabled;
}
void collider::SetEnable(bool n)
{
	_enabled = n;
}
void collider::update()
{
	//if (getGameObject()->getName() == "crate1") std::cout << GetPos() << std::endl;


	SetTrans(getGameObject()->getTransform());
	//go to CheckCollision in collider Manager
	using namespace Engine::Core;
	Transform* transform = getGameObject()->getTransform();



	/*std::vector<collider*> vec = ServiceLocator::instance()->getService<ColliderManager>()->CheckCollision(this);*/
	//vec=ServiceLocator::instance()->getService<ColliderManager>()->CheckCollision(this);
	//if (vec.size() > 0)
		//std::cout << vec[0]->getGameObject()->getTransform()->getPosition() <<" "<< getGameObject()->getName();
}

glm::vec3 collider::GetPos()
{
	return _pos;
}

float collider::GetWidth()
{
	return _width * getGameObject()->getTransform()->getLocalScale().x;
}

float collider::GetHeight()
{
	return _height * getGameObject()->getTransform()->getLocalScale().y;;
}
float collider::GetLength()
{
	return _length * getGameObject()->getTransform()->getLocalScale().z;;
}
float collider::GetRadius()
{
	return _radius * getGameObject()->getTransform()->getLocalScale().x;;
}

void collider::jumpToObj()
{

}

void collider::setI(int i)
{
	index = i;
}

void collider::SetTrans(Transform* t)
{
	_pos = t->getLocalPosition() + glm::vec3();

	glm::mat4 localMatrix = t->getLocalMatrix4X4();
	glm::vec4 rotatedPoint = localMatrix * glm::vec4(0, 0, -1, 1);

	glm::vec4 v4Pos = glm::vec4(_pos, 1);


	/*point1 = localMatrix * (v4Pos + glm::vec4(-GetWidth() / 2, 0, 0.0f, 0.0f));
	point2 = localMatrix * (v4Pos + glm::vec4(GetWidth() / 2, 0, 0.0f, 0.0f));

	point3 = localMatrix * (v4Pos + glm::vec4(0, 0, -GetLength() / 2, 0.0f));
	point4 = localMatrix * (v4Pos + glm::vec4(0, 0, GetLength() / 2, 0.0f));

	point5 = localMatrix * (v4Pos + glm::vec4(0, -GetHeight() / 2, 0, 0.0f));
	point6 = localMatrix * (v4Pos + glm::vec4(0, +GetHeight() / 2, 0, 0.0f));*/


	point1 = localMatrix * (v4Pos + glm::vec4(-1, 0, 0.0f, 0.0f));
	point2 = localMatrix * (v4Pos + glm::vec4(1, 0, 0.0f, 0.0f));

	point3 = localMatrix * (v4Pos + glm::vec4(0, 0, -1, 0.0f));
	point4 = localMatrix * (v4Pos + glm::vec4(0, 0, 1, 0.0f));

	point5 = localMatrix * (v4Pos + glm::vec4(0, -1, 0, 0.0f));
	point6 = localMatrix * (v4Pos + glm::vec4(0, +1, 0, 0.0f));

	normal1 = glm::normalize(_pos - point1);
	normal2 = glm::normalize(_pos - point2);
	normal3 = glm::normalize(_pos - point3);
	normal4 = glm::normalize(_pos - point4);
	normal5 = glm::normalize(_pos - point5);
	normal6 = glm::normalize(_pos - point6);
}


void collider::prewake()
{
	_colliderManager = Engine::ServiceLocator::instance()->getService<ColliderManager>();
	_colliderManager->addCollider(this);
}

