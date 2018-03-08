#include "collider.h"
#include"ServiceLocator.hpp"
#include "ColliderManager.h"

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
	_height(other._height), _r(other._r)
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
	_r = other._r;
	return *this;
}

void collider::SetType(int t)
{
	_type = box;
}
void collider::SetBoxSize(int w, int h)
{
	if (_type == box)
		_height = h;
	_width = w;
}
void collider::SetSphereRadius()
{
}

void collider::update()
{
	SetTrans(getGameObject()->getTransform());
	//go to CheckCollision in collider Manager

	std::vector<collider*> vec = ServiceLocator::instance()->getService<ColliderManager>()->CheckCollision(this);
	//vec=ServiceLocator::instance()->getService<ColliderManager>()->CheckCollision(this);
	//if (vec.size() > 0)
		//std::cout << vec[0]->getGameObject()->getTransform()->getPosition() <<" "<< getGameObject()->getName();
}

glm::vec3 collider::GetPos()
{
	return _pos;
}

int collider::GetWidth()
{
	return _width;
}

int collider::GetHeight()
{
	return _height;
}
int collider::GetRadius()
{
	return _r;
}

void collider::setI(int i)
{
	index = i;
}

void collider::SetTrans(Transform* t)
{
	_pos = t->getLocalPosition();
	/*_rot=t->glm::qu*/
}


void collider::prewake()
{
	_colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
	_colliderManager->addCollider(this);
}

