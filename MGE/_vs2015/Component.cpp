#include "Component.hpp"
#include "GameObject_.hpp"
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"

namespace Engine
{
	namespace Core
	{
		GameObject_* Component::getGameObject() const
		{
			return _gameObject;
		}

		Transform* Component::getTransform() const
		{
			return getGameObject()->getTransform();
		}

		Component::Component() : _gameObject(nullptr), _enabled(true), _started(false)
		{
		}

		Component::~Component()
			= default;

		Component::Component(const Component& other) : _gameObject(other._gameObject), _enabled(other._enabled),
		                                               _started(false)
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
			if (!enable) _started = false;
		}

		void Component::destroy()
		{
			_gameObject->removeComponent(this);
			delete this;
		}

		void Component::setGameObject(GameObject_* gameObject)
		{
			_gameObject = gameObject;
			if (gameObject == nullptr) std::cout << "GameObject null " << std::endl;
			//std::cout << "Awake() is in setGameObject()" << std::endl;
			prewake();
			awake();

			if (ServiceLocator::instance()->getService<GameLoop>()->hasStarted())
			{
				_started = true;
				start();
			}
		}

		bool Component::isUniquePerGameObject()
		{
			return true;
		}
	}
}
