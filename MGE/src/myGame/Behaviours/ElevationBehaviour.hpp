#ifndef ELEVATIONBEHAVIOUR_HPP
#define ELEVATIONBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"
#include <SFML\System.hpp>

/**
* ElevationBehaviour allows you to move a camera using the mouse in its own
* local space in a orbital view around a specified target object.
* Left right rotates, forward back tilts, scroll wheel zooms.
*/
class ElevationBehaviour : public AbstractBehaviour
{
public:
	//tilt speed and turn speed are degrees per second
	ElevationBehaviour(float speed = 1);
	virtual ~ElevationBehaviour();
	virtual void update(float pStep);

private:
	float _speed;
};

#endif // ELEVATIONBEHAVIOUR_HPP