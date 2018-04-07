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
	//std::cout << "State: " + std::to_string(_playerS) << std::endl;
	Camera_::getMainCamera()->getGameObject()->getTransform()->
		setPosition(getGameObject()->getTransform()->getPosition() + glm::vec3(-10, 10, 10));

	Camera_::getMainCamera()->getGameObject()->getTransform()->
		lookAt(getGameObject()->getTransform(), glm::vec3(0, 1, 0));


	getGameObject()->getComponent<collider>()->lastPos = getGameObject()->getTransform()->getPosition();
	//std::cout << getGameObject()->getTransform()->getPosition() << std::endl;
	using namespace Engine::Core;
	using namespace Engine::Utility;
	Transform* transform = getGameObject()->getTransform();

	RayCast();
	float speed = 8 * Engine::Utility::Time::deltaTime();
	float turnSpeed = 90 * Engine::Utility::Time::deltaTime();
	lastPos = transform->getPosition();
	if (_playerS != jumpingToObject)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			transform->translate(transform->forward() * -speed);
			if (_playerS == usingObject)
				_objectToMove->getTransform()->translate(transform->forward() * -speed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			transform->translate(transform->forward() * speed);
			if (_playerS == usingObject)
				_objectToMove->getTransform()->translate(transform->forward() * speed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			transform->rotate(glm::vec3(0, 1, 0), glm::radians(turnSpeed));
			if (_playerS == usingObject)
				_objectToMove->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(turnSpeed));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			transform->rotate(glm::vec3(0, 1, 0), glm::radians(-turnSpeed));
			if (_playerS == usingObject)
				_objectToMove->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(-turnSpeed));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			transform->translate(transform->up() * speed);
			if (_playerS == usingObject)
				_objectToMove->getTransform()->translate(transform->up() * speed);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			transform->translate(transform->up() * -speed);
			if (_playerS == usingObject)
				_objectToMove->getTransform()->translate(transform->up() * -speed);
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && _playerS == usingObject)
	{
		_playerS = jumpingFromObject;
	}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) Camera_::getMainCamera()->getGameObject()->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(30.0f));
	//transform->rotate(glm::vec3(1, 1, 0), glm::radians(turnSpeed));

	if (_playerS == jumpingToObject)
	{
		getGameObject()->getTransform()->setScale(glm::lerp(getGameObject()->getTransform()->getScale(), _targetScale, 0.1f));
		MoveInsideObj(_objectToMove);
	}
	if (_playerS == jumpingFromObject)
	{
		//hardcode
		std::cout << "jumping from" << std::endl;
		getGameObject()->getTransform()->setScale(glm::lerp(getGameObject()->getTransform()->getScale(), _originalScale, 0.1f));
		getGameObject()->getTransform()->setPosition(glm::lerp(getGameObject()->getTransform()->getPosition(), 
			_objectToMove->getTransform()->getPosition() + glm::vec3(2, +2, 2), 0.05f));
		std::cout << " Length" << glm::length(glm::vec3(
			getGameObject()->getTransform()->getPosition().x - _objectToMove->getTransform()->getPosition().x - 2,
			getGameObject()->getTransform()->getPosition().y - _objectToMove->getTransform()->getPosition().y + 2,
			getGameObject()->getTransform()->getPosition().z - _objectToMove->getTransform()->getPosition().z - 2)) << std::endl;
		if (glm::length(glm::vec3(
			getGameObject()->getTransform()->getPosition().x - _objectToMove->getTransform()->getPosition().x - 2,
			getGameObject()->getTransform()->getPosition().y - _objectToMove->getTransform()->getPosition().y - 2,
			getGameObject()->getTransform()->getPosition().z - _objectToMove->getTransform()->getPosition().z - 2)) < 1.0f)
		{
			getGameObject()->getComponent<collider>()->SetEnable(true);
			_objectToMove->getComponent<collider>()->SetEnable(true);
			_playerS = idle;
		}

	}
}

void PlayerBaseComponent::start()
{
	_originalScale = getGameObject()->getTransform()->getLocalScale();
	_targetScale = _originalScale / 80;
}

void PlayerBaseComponent::MoveInsideObj(GameObject_* obj)
{
	getGameObject()->getComponent<collider>()->SetEnable(false);
	//obj->getComponent<collider>()->SetEnable(false);
	getGameObject()->getTransform()->setParent(nullptr);
	obj->getTransform()->setParent(nullptr);
	//getGameObject()->getTransform()->setPosition(obj->getTransform()->getPosition());

	//getGameObject()->getTransform()->setPosition(glm::lerp(
	//	getGameObject()->getTransform()->getPosition(), obj->getTransform()->getPosition(), 1.0f));
	getGameObject()->getTransform()->setPosition(moveTowards(
		getGameObject()->getTransform()->getPosition(), obj->getTransform()->getPosition(), 1.0f));
	//const auto check = glm::epsilonEqual(getGameObject()->getTransform()->getPosition(),
	//	getGameObject()->getComponent<collider>()->lastPos, 0.01f);
	//if (check.x && check.y && check.z)
	//if (glm::distance(getGameObject()->getTransform()->getPosition(),
	//	getGameObject()->getComponent<collider>()->lastPos) < 0.01f)
	//	getGameObject()->getTransform()->setPosition(_targetPosition);

	//glm::vec3 delta = ServiceLocator::instance()->getService<ColliderManager>()->
	//	GiveVectorBeetweenObjects(getGameObject(), obj);
	//const glm::vec3 delta = obj->getTransform()->getPosition() - getGameObject()->getTransform()->getPosition();
	const glm::vec3 delta = _targetScale - getGameObject()->getTransform()->getLocalScale();

	//std::cout << glm::to_string(getGameObject()->getTransform()->getPosition()) << std::endl;
	//std::cout << glm::to_string(obj->getTransform()->getPosition()) << std::endl;
	//std::cout << glm::to_string(getGameObject()->getComponent<collider>()->lastPos) << std::endl;
	//std::cout << "Delta length: " + std::to_string(glm::length(delta)) << std::endl;
	//std::cout << (glm::abs(delta.length() - glm::length(delta)) < 0.000001f ? "true" : "false") << std::endl;
	//if (glm::length(delta) <= 0.1f)
	const auto check = glm::epsilonEqual(getGameObject()->getTransform()->getLocalScale(),
		_targetScale, glm::epsilon<float>());
	if (check.x && check.y && check.z)
	{
		_playerS = usingObject;
	}
}

glm::vec3 PlayerBaseComponent::moveTowards(glm::vec3 current, glm::vec3 target, float maxDelta)
{
	glm::vec3 delta = target - current;
	glm::vec3 movementDelta = glm::length2(delta) > maxDelta * maxDelta ? glm::normalize(delta) * maxDelta : delta;
	return current + movementDelta;
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
	for (unsigned i = 0; i < ServiceLocator::instance()->getService<ColliderManager>()->getColliders().size(); i++)
	{
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
		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			std::cout << "dist: " << distance << std::endl;
		std::cout << child->getGameObject()->getName() << std::endl;
		}*/
		//I know the shere radius is 1, this needs to be replaced with collider radius

		if (distance <= child->GetRadius() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			//std::cout << "dist is 1" << std::endl;
			if (child->getGameObject()->getName() == "Runestone")
			{
				_objectToMove = child->getGameObject();
				_targetPosition = _objectToMove->getTransform()->getPosition();
				_playerS = jumpingToObject;
				getGameObject()->getComponent<collider>()->SetEnable(false);
				//std::cout << "jumping" << std::endl;
			}
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
