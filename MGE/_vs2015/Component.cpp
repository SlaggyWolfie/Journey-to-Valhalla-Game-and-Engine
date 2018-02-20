#include "Component.hpp"
#include "GameObject_.hpp"

namespace Engine
{
	namespace Core
	{
		GameObject_* Component::getGameObject() const
		{
			return _gameObject;
		}

		Component::Component() : _gameObject(nullptr), _enabled(true)
		{
		}

		Component::~Component()
			= default;

		Component::Component(const Component& other) : _gameObject(other._gameObject), _enabled(other._enabled)
		{
		}

		Component& Component::operator=(const Component& other)
		{
			_gameObject = other._gameObject;
			return *this;
		}

		bool Component::isEnabled() const
		{
			return _enabled;
		}

		void Component::setEnabled(const bool enable)
		{
			_enabled = enable;
		}

		void Component::destroy()
		{
			_gameObject->removeComponent(this);
		}

		void Component::setGameObject(GameObject_* gameObject)
		{
			_gameObject = gameObject;
			std::cout << "Awake() is in setGameObject()" << std::endl;
			prewake();
			awake();
		}

		bool Component::isUniquePerGameObject()
		{
			return true;
		}
	}
}
