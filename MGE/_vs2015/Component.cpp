#include "Component.hpp"

namespace Engine
{
	GameObject_* Component::getGameObject() const
	{
		return _gameObject;
	}

	Component::Component() :_gameObject(nullptr)
	{
	}

	Component::~Component()
		= default;

	Component::Component(const Component& other) : _gameObject(other._gameObject)
	{
	}

	Component& Component::operator=(const Component& other)
	{
		_gameObject = other._gameObject;
		return *this;
	}

	void Component::setGameObject(GameObject_* gameObject)
	{
		_gameObject = gameObject;
	}

	bool Component::isUniquePerGameObject()
	{
		return true;
	}
}
