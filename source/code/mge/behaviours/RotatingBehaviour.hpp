#ifndef ROTATINGBEHAVIOUR_HPP
#define ROTATINGBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Simply rotates the object around its origin with a fixed speed.
 */
class RotatingBehaviour : public AbstractBehaviour
{
	public:
		RotatingBehaviour();
		virtual ~RotatingBehaviour();

		virtual void update(float pStep);

};

#endif // ROTATINGBEHAVIOUR_HPP
