#pragma once
#include "Component.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <glm.hpp>

class RotatingComponent : public Engine::Core::Component
{
public:
	RotatingComponent() = default;
	virtual ~RotatingComponent() = default;

	RotatingComponent(const RotatingComponent& other) = default;
	RotatingComponent& operator=(const RotatingComponent& other) = default;

	void destroy() override
	{
		delete this;
	}

protected:
	void update() override
	{
		auto transform = getGameObject()->getTransform();
		transform->rotate(transform->up() + transform->right(), glm::radians(1.0f));
	}
};