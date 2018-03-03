#include "collider.h"
#include"ServiceLocator.hpp"


collider::collider(colliderType pType): _colliderManager(nullptr), _t(nullptr), _type(pType)
{
}


collider::~collider()
{
}
void collider::SetBoxSize(int w,int h)
{
	if(_type == box)
	_height = h;
	_width = w;
}
void collider::SetSphereRadius(int r)
{
	if (_type == sphere)
		_r = r;
}

void collider::update()
{
	SetTrans(getGameObject()->getTransform());
	//this is smart stuff
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

