#include "collider.h"
#include"ServiceLocator.hpp"
#include "ColliderManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/Game.hpp"
#include "Time.hpp"
#include "PlayerBaseComponent.h"
#include "Camera_.hpp"
collider::collider() : _colliderManager(nullptr), _t(nullptr)
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

collider::~collider()
{
}
void collider::SetType(int t)
{
	_type = sphere;
}
void collider::SetBoxSize(int w, int h)
{
}
void collider::SetSphereRadius()
{
}

void collider::fixedUpdate()
{
}
void collider::PushBackObj(std::string n, std::string n1)
{
	std::cout << n << " " << n1 << std::endl;
	ColliderManager* c = ServiceLocator::instance()->getService<ColliderManager>();
	GameObject_* obj1 = c->
		GetColliderByName(n)->getGameObject();
	GameObject_* obj2 = c->
		GetColliderByName(n1)->getGameObject();
	glm::vec3 l = c->GiveVectorBeetweem(c-> GetColliderByName(n), c->GetColliderByName(n1));
	std::cout << l << std::endl;
	obj1->getTransform()->setPosition(glm::lerp(obj1->getTransform()->getPosition(), c->
		GetColliderByName(n)->lastPos+glm::vec3(l.x*0.3f,l.y*0.3,l.z*0.3f), 0.1f));
	
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
	jumpToObj();
	SetTrans(getGameObject()->getTransform());
	//go to CheckCollision in collider Manager
	using namespace Engine::Core;
	Transform* transform = getGameObject()->getTransform();
	float speed = 2.0f;
	float turnSpeed = 3.0f;
	lastPos = transform->getPosition();
	//getGameObject()->getComponent<LastposStasher>()->SetLastPos(transform->getPosition());
	if (getGameObject()->getName() == "Player")
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) transform->translate(transform->forward() * speed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) transform->translate(transform->forward() * -speed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) transform->translate(transform->right() * -speed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) transform->translate(transform->right() * speed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) transform->translate(transform->up() * speed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) transform->translate(transform->up() * -speed);
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) Camera_::getMainCamera()->getGameObject()->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(30.0f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) Camera_::getMainCamera()->getGameObject()->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(3.0f));
			//transform->rotate(glm::vec3(1, 1, 0), glm::radians(turnSpeed));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
		{
			std::cout<<this->getGameObject()->getComponent<collider>()->lastPos<<std::endl;
			transform->rotate(glm::vec3(1, 1, 0), glm::radians(-turnSpeed));
		}


		
		//std::cout << vec[0]->getGameObject()->getTransform()->getPosition() <<" "<< getGameObject()->getName();
	}
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

void collider::jumpToObj()
{
	sf::Vector2u windowSize = ServiceLocator::instance()->getService<Game>()->getWindow()->getSize();
	//first get mouse position, which will be between (0,0) and (windowSize.x, windowSize.y)
	//game is a service i can get it via service locator

	sf::Vector2i mousePosition = sf::Mouse::getPosition(*ServiceLocator::instance()->getService<Game>()->getWindow());
	//but we want the mouse position relative to center of the screen, that is where the camera is pointing
	glm::vec2 mousePosRelativeToScreenCenter = glm::vec2(
		(float)mousePosition.x - (windowSize.x / 2),
		(float)-mousePosition.y + (windowSize.y / 2)
	);

	//now we calculate the ratio between the screen height and nearPlane height using the vertical fov passed into the projection matrix (see Camera.hpp)
	//noting that since we fill in the -nearPlane in the rayNearPlane, no matter our value of nearPlane, the ratio's will still be correct,
	//we can simply use 1 for the nearPlane value, simpliyfing everything even further
	float verticalFOV = 45.0f;  //taken from Camera.hpp
	float nearPlaneHeight = 2 * tan(glm::radians(verticalFOV / 2.0f));
	float ratio = nearPlaneHeight / windowSize.y;

	//get the ray coordinates using mouse coordinates relative to the screen center by multiplying them with the ratio
	glm::vec4 rayNearPlane = glm::vec4(
		mousePosRelativeToScreenCenter.x * ratio,
		mousePosRelativeToScreenCenter.y * ratio,
		-1,
		0
	);

	//see where this ray is actually pointing in the world and normalize it so we can use it for projection
	glm::vec3 rayWorld = glm::vec3(Camera_::getMainCamera()->getGameObject()->getTransform()->getMatrix4X4() * rayNearPlane);
	rayWorld = glm::normalize(rayWorld);

	std::cout << ServiceLocator::instance()->getService<ColliderManager>()->getColliders().size() << std::endl;
	//fake collision loop in here
	for (int i = 0; i < ServiceLocator::instance()->getService<ColliderManager>()->getColliders().size(); i++) {
		collider* child = ServiceLocator::instance()->getService<ColliderManager>()->getColliders()[i];
		//if (child == _world->getMainCamera()) continue;

		//get the vector from camera to object
		glm::vec3 cameraToSphere(child->GetPos() - Camera_::getMainCamera()->getGameObject()->getTransform()->getPosition());
		//project that vector onto the ray so we have the part of cameraToSphere along the ray
		glm::vec3 parallel = glm::dot(cameraToSphere, rayWorld) * rayWorld;
		//subtract that part from the vector to get the vector parallel to our ray
		glm::vec3 perpendicular = cameraToSphere - parallel;
		//and get its distance
		float distance = glm::length(perpendicular);

		//I know the shere radius is 1, this needs to be replaced with collider radius
		if (distance <= child->GetRadius() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

			std::cout << child->getGameObject()->getName() << std::endl;
			if(this->getGameObject()->getName()=="Player")
			this->getGameObject()->getTransform()->setPosition(glm::lerp(this->getGameObject()->getTransform()->getPosition(), child->getGameObject()->getTransform()->getPosition(), 0.1f));

		}
		/*else {
		child->setMaterial(_normalMaterial);
		}*/
	}
}

void collider::setI(int i)
{
	index = i;
}

void collider::SetTrans(Transform* t)
{
	_pos = t->getLocalPosition();

	glm::mat4 localMatrix = t->getLocalMatrix4X4();
	glm::vec4 rotatedPoint = localMatrix * glm::vec4(0, 0, -1, 1);

	glm::vec4 v4Pos=  glm::vec4(_pos, 1);
	point1 = localMatrix * (v4Pos + glm::vec4(-_width / 2, 0, 0.0f,0.0f));
	point2 = localMatrix * (v4Pos + glm::vec4(_width / 2, 0, 0.0f, 0.0f));

	point3 = localMatrix * (v4Pos + glm::vec4(0, 0, -_width/2, 0.0f));
	point4 = localMatrix * (v4Pos + glm::vec4(0, 0, _width/2, 0.0f));

	point5 = localMatrix * (v4Pos + glm::vec4(0, -_height / 2, 0, 0.0f));
	point6 = localMatrix * (v4Pos + glm::vec4(0, +_height / 2, 0, 0.0f));
}


void collider::prewake()
{
	_colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
	_colliderManager->addCollider(this);
}

