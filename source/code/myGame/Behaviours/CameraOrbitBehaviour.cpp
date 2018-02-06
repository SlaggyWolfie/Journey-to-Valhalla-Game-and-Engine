#include "CameraOrbitBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <iostream>
#include <string>

CameraOrbitBehaviour::CameraOrbitBehaviour(
	GameObject * target,
	float tiltSpeed, float minTiltAngle, float maxTiltAngle,
	float turnSpeed,
	float startDistanceFromTarget, float minDistance, float maxDistance) :
	AbstractBehaviour(),
	_target(target),
	_tiltSpeed(tiltSpeed), _minTiltAngle(minTiltAngle), _maxTiltAngle(maxTiltAngle),
	_turnSpeed(turnSpeed),
	_distance(startDistanceFromTarget), _minDistance(minDistance), _maxDistance(maxDistance),
	_lastMousePosition(sf::Vector2i(0, 0)), _tilt(0), _turn(45), _initialized(false)
{
}

CameraOrbitBehaviour::~CameraOrbitBehaviour()
{
	delete _target;
}

//void CameraOrbitBehaviour::setScale(glm::vec3 scalar)
//{
//	_scalar = scalar;
//}

void CameraOrbitBehaviour::update(float pStep)
{
	if (_initialized)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition();
		sf::Vector2i mouseDelta = _lastMousePosition - mousePosition;
		//_lastMousePosition = sf::Vector2i(960, 540);
		_lastMousePosition = mousePosition;

		//mouseDelta = sf::Vector2i(0, 0);
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) mouseDelta.x = -1;
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) mouseDelta.y = -1;

		//if (mouseDelta.x == 0 && mouseDelta.y == 0) return;

		//_turn += mouseDelta.x / 960 * _turnSpeed * pStep;
		//_tilt += mouseDelta.y / 540 * _tiltSpeed * pStep;

		if (mouseDelta.x > 0) _turn += _turnSpeed * pStep;
		else if (mouseDelta.x < 0) _turn -= _turnSpeed * pStep;

		if (mouseDelta.y > 0) _tilt += _tiltSpeed * pStep;
		else if (mouseDelta.y < 0) _tilt -= _tiltSpeed * pStep;
	}
	else 
	{
		_initialized = true;
	}

	//Clamp
	_tilt = _clamp(_tilt, _minTiltAngle, _maxTiltAngle);
	_turn = _wrap(_turn, 0, 360);
	//_distance = _clamp(_distance, _minDistance, _maxDistance);

	_rotate(_turn, _tilt, _distance);
}

float CameraOrbitBehaviour::_clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

float CameraOrbitBehaviour::_wrap(float value, float min, float max)
{
	if (min > max)
	{
		std::cout << "Error max is less than min";
		return 0;
	}
	while (value < min) value += max;
	while (value > max) value -= max;
	return value;
}

glm::mat4x4 CameraOrbitBehaviour::_lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
	glm::vec3 forward = -glm::normalize(target - eye);
	glm::vec3 side = glm::normalize(glm::cross(up, forward));
	glm::vec3 upward = glm::normalize(glm::cross(forward, side));

	return glm::mat4(glm::vec4(side, 0), glm::vec4(upward, 0), glm::vec4(forward, 0), glm::vec4(0, 0, 0, 1));
}

void CameraOrbitBehaviour::_rotate(float turn, float tilt, float distance, bool debug)
{
	float degreeToRadians = 3.14159265f / 180;
	float cosTilt = std::cosf(tilt * degreeToRadians);
	float sinTilt = std::sinf(tilt * degreeToRadians);
	float cosTurn = std::cosf(turn * degreeToRadians);
	float sinTurn = std::sinf(turn * degreeToRadians);

	glm::vec3 lookAtPosition = _target->getWorldPosition();
	float x = lookAtPosition.x + distance * sinTurn * sinTilt;
	float y = lookAtPosition.y + distance * cosTilt;
	float z = lookAtPosition.z - distance * cosTurn * sinTilt;

	glm::vec3 lookFromPosition = glm::vec3(x, y, z);

	glm::mat4x4 transform = _lookAt(lookFromPosition, lookAtPosition, glm::vec3(0, 1, 0));
	transform[3] = glm::vec4(lookFromPosition, 1);
	glm::vec3 scalar = _owner->getScale();
	if (scalar != glm::vec3(0, 0, 0)) transform = glm::scale(transform, scalar);
	_owner->setTransform(transform);

	if (debug)
	{
		glm::vec3 ownerLPos = _owner->getLocalPosition();
		std::cout << std::string("Tilt: ") + std::to_string(tilt) << std::endl;
		std::cout << std::string("Turn: ") + std::to_string(turn) << std::endl;
		std::cout << std::string("Owner Pos X: ") + std::to_string(ownerLPos.x) << std::endl;
		std::cout << std::string("Owner Pos Y: ") + std::to_string(ownerLPos.y) << std::endl;
		std::cout << std::string("Owner Pos Z: ") + std::to_string(ownerLPos.z) << std::endl;
		std::cout << std::string("X: ") + std::to_string(x) << std::endl;
		std::cout << std::string("Y: ") + std::to_string(y) << std::endl;
		std::cout << std::string("Z: ") + std::to_string(z) << std::endl;
		std::cout << std::endl;
	}
}
