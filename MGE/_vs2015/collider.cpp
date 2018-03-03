#include "collider.h"
#include"ServiceLocator.hpp"
#include "ColliderManager.h"

collider::collider(): _colliderManager(nullptr), _t(nullptr)
{
}


collider::~collider()
{
}
void collider::SetType(int t)
{
	_type = box;
}
void collider::SetBoxSize(int w,int h)
{
	if(_type == box)
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
	ServiceLocator::instance()->getService<ColliderManager>()->CheckCollision(this);
}

glm::vec3 collider::GetPos()
{
	return _pos;
}

int collider::GetWidth()
{
	if (_type == box)
	return _width;
}

int collider::GetHeight()
{
	if (_type == box)
	return _height;
}
int collider::GetRadius()
{
	if (_type == sphere)
	return _r;
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

