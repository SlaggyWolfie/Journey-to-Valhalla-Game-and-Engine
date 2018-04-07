#include "collider.h"
#include"ServiceLocator.hpp"
#include "ColliderManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/Game.hpp"
#include "Time.hpp"
#include "PlayerBaseComponent.h"
#include "Camera_.hpp"
#include "GL\glew.h"
#include "GameObject_.hpp"
#include "Model.hpp"

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		GameObject_* crate = Model::loadModel(File::findPath("Crate.fbx"));
		crate->getTransform()->setLocalMatrix4X4(getGameObject()->getTransform()->getMatrix4X4());
		crate->getTransform()->setScale(halfSize * 2);

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
	{
		//std::cout << getGameObject()->getName() << std::endl;
		//std::cout << corner1 << std::endl;
		//std::cout << corner7 << std::endl;
		std::cout << glewGetErrorString(glGetError()) << std::endl;
	}

	//debugRender(Core::Camera_::getMainCamera()->getProjectionMatrix(), Core::Camera_::getMainCamera()->getViewMatrix());



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
	return _width;
}

float collider::GetHeight()
{
	return _height;
}
float collider::GetLength()
{
	return _length;
}
float collider::GetRadius()
{
	return _radius;
}

void collider::jumpToObj()
{

}

void collider::setI(int i)
{
	index = i;
}


glm::vec3 collider::leftBottomBack()
{
	return glm::vec3(-1);
}

glm::vec3 collider::rightBottomBack()
{
	return glm::vec3(1, -1, -1);
}

glm::vec3 collider::leftTopBack()
{
	return glm::vec3(-1, 1, -1);
}

glm::vec3 collider::rightTopBack()
{
	return glm::vec3(1, 1, -1);
}

glm::vec3 collider::leftBottomFront()
{
	return glm::vec3(-1, -1, 1);
}

glm::vec3 collider::rightBottomFront()
{
	return glm::vec3(1, -1, 1);
}

glm::vec3 collider::leftTopFront()
{
	return glm::vec3(-1, 1, 1);
}

glm::vec3 collider::rightTopFront()
{
	return glm::vec3(1);
}

glm::vec3 collider::getLocalPosition(int index)
{
	index--;
	switch (index)
	{
	case 0: return leftBottomBack();
	case 1: return rightBottomBack();
	case 2: return rightBottomFront();
	case 3: return leftBottomFront();
	case 4: return leftTopBack();
	case 5: return rightTopBack();
	case 6: return rightTopFront();
	case 7: return leftTopFront();
	default: std::cout << "Nani the fuck!?" << std::endl; break;
	}

	return glm::vec3(0);
}

void collider::debugRender(glm::mat4 proj, glm::mat4 view)
{
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(proj));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(view * /*Core::Camera_::getMainCamera()->*/getGameObject()->getTransform()->getMatrix4X4()));
	//glLoadMatrixf(glm::value_ptr(view * getGameObject()->getTransform()->getMatrix4X4()));

	glBegin(GL_LINES);
	//glColor3fv(glm::value_ptr(glm::vec3(1,0,0)));
	drawLine(glm::vec3(0, -500, -1), glm::vec3(0, 500, -1));

	drawLine(corner1, corner2);
	drawLine(corner2, corner3);
	drawLine(corner3, corner4);
	drawLine(corner4, corner1);
	drawLine(corner5, corner6);
	drawLine(corner6, corner7);
	drawLine(corner7, corner8);
	drawLine(corner8, corner5);
	drawLine(corner1, corner5);
	drawLine(corner2, corner6);
	drawLine(corner3, corner7);
	drawLine(corner4, corner8);

	glEnd();
}

void collider::drawLine(glm::vec3 point1, glm::vec3 point2)
{
	glColor3fv(glm::value_ptr(glm::vec3(1, 0, 0)));
	glVertex3fv(glm::value_ptr(point1));
	glVertex3fv(glm::value_ptr(point2));
}

void collider::renderDebug()
{
	debugRender(Core::Camera_::getMainCamera()->getProjectionMatrix(), Core::Camera_::getMainCamera()->getViewMatrix());
	//std::cout << "Hello" << std::endl;
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


	//point1 = localMatrix * (v4Pos + glm::vec4(-1, 0, 0.0f, 0.0f));
	//point2 = localMatrix * (v4Pos + glm::vec4(1, 0, 0.0f, 0.0f));

	//point3 = localMatrix * (v4Pos + glm::vec4(0, 0, -1, 0.0f));
	//point4 = localMatrix * (v4Pos + glm::vec4(0, 0, 1, 0.0f));

	//point5 = localMatrix * (v4Pos + glm::vec4(0, -1, 0, 0.0f));
	//point6 = localMatrix * (v4Pos + glm::vec4(0, +1, 0, 0.0f));	

	point1 = localMatrix * glm::vec4(-1, 0, 0.0f, 1);
	point2 = localMatrix * glm::vec4(1, 0, 0.0f, 1);

	point3 = localMatrix * glm::vec4(0, 0, -1, 1);
	point4 = localMatrix * glm::vec4(0, 0, 1, 1);

	point5 = localMatrix * glm::vec4(0, -1, 0, 1);
	point6 = localMatrix * glm::vec4(0, +1, 0, 1);

	normal1 = glm::normalize(_pos - point1);
	normal2 = glm::normalize(_pos - point2);
	normal3 = glm::normalize(_pos - point3);
	normal4 = glm::normalize(_pos - point4);
	normal5 = glm::normalize(_pos - point5);
	normal6 = glm::normalize(_pos - point6);

	//halfSize = glm::vec3(GetWidth(), GetHeight(), GetLength()) / 2;
	//halfSize = glm::vec3(localMatrix * glm::vec4(glm::vec3(1.0f) / 2, 0));
	halfSize = glm::vec3(GetWidth(), GetHeight(), GetLength()) / 2;

	corner1 = /*t->getPosition() + */halfSize * getLocalPosition(1);
	corner2 = /*t->getPosition() + */halfSize * getLocalPosition(2);
	corner3 = /*t->getPosition() + */halfSize * getLocalPosition(3);
	corner4 = /*t->getPosition() + */halfSize * getLocalPosition(4);
	corner5 = /*t->getPosition() + */halfSize * getLocalPosition(5);
	corner6 = /*t->getPosition() + */halfSize * getLocalPosition(6);
	corner7 = /*t->getPosition() + */halfSize * getLocalPosition(7);
	corner8 = /*t->getPosition() + */halfSize * getLocalPosition(8);

	//corner1 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(1), 1));
	//corner2 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(2), 1));
	//corner3 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(3), 1));
	//corner4 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(4), 1));
	//corner5 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(5), 1));
	//corner6 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(6), 1));
	//corner7 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(7), 1));
	//corner8 = glm::vec3(localMatrix * glm::vec4(halfSize * getLocalPosition(8), 1));
}


void collider::prewake()
{
	_colliderManager = Engine::ServiceLocator::instance()->getService<ColliderManager>();
	_colliderManager->addCollider(this);
}

