#ifndef CAMERAORBITBEHAVIOUR_HPP
#define CAMERAORBITBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include <SFML\System.hpp>

/**
* CameraOrbitBehaviour allows you to move a camera using the mouse in its own
* local space in a orbital view around a specified target object.
* Left right rotates, forward back tilts, scroll wheel zooms.
*/
class CameraOrbitBehaviour : public AbstractBehaviour
{
public:
	//tilt speed and turn speed are degrees per second
	CameraOrbitBehaviour(GameObject* target,
		float tiltSpeed = 15, float minTiltAngle = 1, float maxTiltAngle = 75, 
		float turnSpeed = 15,
		float startDistanceFromTarget = 3, float minDistance = 1, float maxDistance = 10);
	virtual ~CameraOrbitBehaviour();
	virtual void update(float pStep);
	//void setScale(glm::vec3 scalar = glm::vec3(1));

private:
	bool _initialized;
	sf::Vector2i _lastMousePosition;
	float _turn;
	float _tilt;

	GameObject* _target;
	float _tiltSpeed;
	float _turnSpeed;
	float _minTiltAngle;
	float _maxTiltAngle;
	float _minDistance;
	float _maxDistance;
	float _distance;

	float _clamp(float value, float min, float max);
	float _wrap(float value, float min, float max);
	void _rotate(float turn, float tilt, float distance, bool debug = false);
	glm::mat4x4 _lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);

	//glm::vec3 _scalar;
};

#endif // CAMERAORBITBEHAVIOUR_HPP