#include "ElevationBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <iostream>
#include <string>

ElevationBehaviour::ElevationBehaviour(float speed) : _speed(speed)
{
}

ElevationBehaviour::~ElevationBehaviour()
{
}
void ElevationBehaviour::update(float pStep)
{
	float moveSpeed = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		moveSpeed = _speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		moveSpeed = -_speed;
	}
	//translate the object in its own local space
	_owner->translate(glm::vec3(0.0f, moveSpeed*pStep, 0));
}