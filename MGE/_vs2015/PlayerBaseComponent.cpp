#include "PlayerBaseComponent.h"
#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"
#include "LastposStasher.h"

PlayerBaseComponent::PlayerBaseComponent()
{
}

void PlayerBaseComponent::update()
{
	using namespace Engine::Core;
	Transform* transform = getGameObject()->getTransform();
	float speed = 2.0f;
	float turnSpeed = 3.0f;
	lp = transform->getPosition();
	//getGameObject()->getComponent<LastposStasher>()->SetLastPos(transform->getPosition());
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) transform->translate(transform->forward() * speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) transform->translate(transform->forward() * -speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) transform->translate(transform->right() * -speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) transform->translate(transform->right() * speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) transform->translate(transform->up() * speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) transform->translate(transform->up() * -speed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) transform->rotate(glm::vec3(0, 1, 0), glm::radians(turnSpeed));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) transform->rotate(glm::vec3(0, 0, -1), glm::radians(-turnSpeed));
}

void PlayerBaseComponent::destroy()
{
	delete this;
}

bool PlayerBaseComponent::isUniquePerGameObject()
{
	return true;
}
