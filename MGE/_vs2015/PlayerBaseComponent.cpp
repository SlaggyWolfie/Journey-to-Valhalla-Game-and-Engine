#include "PlayerBaseComponent.h"
#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"
using namespace Engine::Core;
using namespace Engine;


PlayerBaseComponent::PlayerBaseComponent()
{
}

void PlayerBaseComponent::update()
{

	Camera_::getMainCamera()->getGameObject()->getTransform()->
		lookAt(this->getGameObject()->getTransform(), glm::vec3(0, 1, 0));

	Camera_::getMainCamera()->getGameObject()->getTransform()->
		setPosition(this->getGameObject()->getTransform()->getPosition() + glm::vec3(-10, 5, 0));

	using namespace Engine::Core;
	Transform* transform = getGameObject()->getTransform();

	RayCast();
	float speed = 4 * Engine::Utility::Time::deltaTime();
	float turnSpeed = 0.001f*Engine::Utility::Time::deltaTime();
	lastPos = transform->getPosition();
	//getGameObject()->getComponent<LastposStasher>()->SetLastPos(transform->getPosition());
	if (_playerS != jumpingToObject)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (_playerS == usingObject)
			{
				_objectToMove->getTransform()->translate(transform->forward() * speed);
				transform->translate(transform->forward() * speed);
			}
			else
			{
				transform->translate(transform->forward() * speed);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (_playerS == usingObject)
			{
				transform->translate(transform->forward() * -speed);
				_objectToMove->getTransform()->translate(transform->forward() * -speed);
			}
			else
			{
				transform->translate(transform->forward() * -speed);
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if (_playerS == usingObject)
			{
				//transform->translate(transform->right() * speed);
				//_objectToMove->getTransform()->translate(transform->right() * speed);
				transform->rotate(glm::vec3(0, 1, 0), -turnSpeed);
				_objectToMove->getTransform()->rotate(glm::vec3(0, 1, 0), -turnSpeed);

			}
			else
			{
				transform->rotate(glm::vec3(0, 1, 0), -turnSpeed);

			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (_playerS == usingObject)
			{
				//transform->translate(transform->right() * speed);
				//_objectToMove->getTransform()->translate(transform->right() * speed);
				transform->rotate(glm::vec3(0, 1, 0), turnSpeed);
				_objectToMove->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(turnSpeed));

			}
			else
			{
				transform->rotate(glm::vec3(0, 1, 0), glm::radians(turnSpeed), World);

			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			if (_playerS == usingObject)
			{
				transform->translate(transform->up() * speed);
				_objectToMove->getTransform()->translate(transform->up() * speed);
			}
			else
			{
				transform->translate(transform->up() * speed);

			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			if (_playerS == usingObject)
			{
				transform->translate(transform->up() * -speed);
				_objectToMove->getTransform()->translate(transform->up() * -speed);
			}
			else
			{
				transform->translate(transform->up() * -speed);

			}

		}
	}
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)) transform->rotate(glm::vec3(0,1,0),turnSpeed);

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) transform->translate(transform->up() * speed);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) transform->translate(transform->up() * -speed);*/
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && _playerS == usingObject)
	{

		_playerS = jumpingFromObject;
	}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) Camera_::getMainCamera()->getGameObject()->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(30.0f));
	//transform->rotate(glm::vec3(1, 1, 0), glm::radians(turnSpeed));

	if (_playerS == jumpingToObject)
	{
		getGameObject()->getTransform()->setScale(glm::lerp(getGameObject()->getTransform()->getScale(), glm::vec3(0.3, 0.3, 0.3), 0.1f));
		MoveInsideObj(_objectToMove);
	}
	if (_playerS == jumpingFromObject)
	{
		//hardcode
		std::cout << "jumping from" << std::endl;
		getGameObject()->getTransform()->setScale(glm::lerp(getGameObject()->getTransform()->getScale(), glm::vec3(1, 1, 1), 0.05f));
		getGameObject()->getTransform()->setPosition(glm::lerp(getGameObject()->getTransform()->getPosition(), _objectToMove->getTransform()->getPosition() + glm::vec3(250, -40, 40), 0.05f));
		std::cout << " Length" << glm::length(glm::vec3(
			getGameObject()->getTransform()->getPosition().x - _objectToMove->getTransform()->getPosition().x - 250,
			getGameObject()->getTransform()->getPosition().y - _objectToMove->getTransform()->getPosition().y + 0,
			getGameObject()->getTransform()->getPosition().z - _objectToMove->getTransform()->getPosition().z - 40)) << std::endl;
		if (glm::length(glm::vec3(
			getGameObject()->getTransform()->getPosition().x - _objectToMove->getTransform()->getPosition().x - 250,
			getGameObject()->getTransform()->getPosition().y - _objectToMove->getTransform()->getPosition().y + 40,
			getGameObject()->getTransform()->getPosition().z - _objectToMove->getTransform()->getPosition().z - 40)) < 5.0f)
		{
			this->getGameObject()->getComponent<collider>()->SetEnable(true);
			_objectToMove->getComponent<collider>()->SetEnable(true);
			_playerS = idle;
		}

	}
}

void PlayerBaseComponent::MoveInsideObj(GameObject_* obj)
{
	this->getGameObject()->getComponent<collider>()->SetEnable(false);
	this->getGameObject()->getTransform()->setPosition(glm::lerp(this->getGameObject()->getTransform()->getPosition(), obj->getTransform()->getPosition(), 0.06f));

	glm::vec3 delta = ServiceLocator::instance()->getService<ColliderManager>()->
		GiveVectorBeetweenObjects(this->getGameObject(), obj);
	//std::cout << (glm::abs(delta.length() - glm::length(delta)) < 0.000001f ? "true" : "false") << std::endl;
	if (glm::length(delta) <= 0.5f)
	{
		_playerS = usingObject;
	}
}

void PlayerBaseComponent::RayCast()
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

	//std::cout << ServiceLocator::instance()->getService<ColliderManager>()->getColliders().size() << std::endl;
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
			//if (child->getGameObject()->getName() == "obj2")
			//{
			_objectToMove = child->getGameObject();
			_playerS = jumpingToObject;
			this->getGameObject()->getComponent<collider>()->SetEnable(false);
			std::cout << "jumping" << std::endl;
			//}
		}
	}
}

bool PlayerBaseComponent::IsInside()
{
	return _insideObj;
}

void PlayerBaseComponent::SetInside(bool n)
{
	_insideObj = n;
}

void PlayerBaseComponent::destroy()
{
	delete this;
}

bool PlayerBaseComponent::isUniquePerGameObject()
{
	return true;
}
