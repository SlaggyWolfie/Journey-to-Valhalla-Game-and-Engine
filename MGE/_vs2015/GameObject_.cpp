#include "GameObject_.hpp"
#include <utility>
//#include "GameObject.hpp"
#include "Transform.hpp"
#include "Component.hpp"

namespace Engine
{
	/**
	 * \brief
	 * \param name Game Object's name.
	 * \param tag Game Object's tag.
	 * \param position Game Object's starting position.
	 */
	GameObject_::GameObject_(std::string name, std::string tag, const glm::vec3 & position)
		: _name(std::move(name)), _tag(std::move(tag)), _isStatic(false)
	{
		_transform = std::make_unique<Transform>();
		_components = std::vector<std::unique_ptr<Component>>();
		_components.push_back(std::make_unique<Component>(*_transform));
	}

	GameObject_::~GameObject_()
	{
		//Search for a component that can be cast to T
		_components.clear();
	}

	template <typename T>
	T* GameObject_::getComponent()
	{
		//Confirm that type T is a component
		if (!std::is_base_of<Engine::Component, T>())
		{
			std::cout << std::string("Type T is not of type Component in getComponent<T>!") << std::endl;
			return nullptr;
		}

		//Search for a component that can be cast to T
		for (int i = 0; i < _components.size(); i++)
		{
			if (dynamic_cast<T>(_components[i].get()) != nullptr)
				return _components[i].get();
		}

		//Return null if we can't find anything
		return nullptr;
	}

	template <typename T>
	void GameObject_::addComponent()
	{
		//Confirm that type T is a component
		if (!std::is_base_of<Component, T>())
		{
			std::cout << std::string("Type T is not of type Component in addComponent<T>!") << std::endl;
			return;
		}

		//Search for a component that can be cast to T
		for (int i = 0; i < _components.size(); i++)
		{
			T* component = dynamic_cast<T>(_components[i].get());
			if (component == nullptr) break;

			if (static_cast<Component>(component).isUniquePerGameObject())
			{
				std::cout << "Attempting to add unique per game object component:" << std::endl;
				return;
			}
		}

		//Might crash
		if (std::is_default_constructible<T>::value)
		{
			T* derivedComponent = new T();
			std::unique_ptr<Component> newComponent = std::make_unique<Component>(derivedComponent);
			newComponent.get()->setGameObject(this);
			_components.push_back(newComponent);
		}
		else std::cout << "Failed to add component. Has no default constructor." << std::endl;
	}

	template <typename T>
	void GameObject_::removeComponent()
	{
		//Confirm that type T is a component
		if (!std::is_base_of<Engine::Component, T>())
		{
			std::cout << std::string("Type T is not of type Component in removeComponent<T>!") << std::endl;
			return;
		}

		//Search for a component that can be cast to T
		for (int i = 0; i < _components.size(); i++)
		{
			Component* component = _components[i].get();
			T* derivedComponent = dynamic_cast<T>(component);
			if (derivedComponent != null)
			{
				_components.erase(
					std::remove(
						_components.begin(), _components.end(), component
					), _components.end());
				delete component;
				return;
			}
		}
	}

	std::string GameObject_::getName() const
	{
		return _name;
	}

	void GameObject_::setName(const std::string& name)
	{
		_name = name;
	}

	std::string GameObject_::getTag() const
	{
		return _tag;
	}

	void GameObject_::setTag(const std::string& tag)
	{
		_tag = tag;
	}

	Transform* GameObject_::getTransform() const
	{
		return _transform.get();
	}

	bool GameObject_::isStatic() const
	{
		return _isStatic;
	}

	bool GameObject_::setStatic(const bool isStatic)
	{
		_isStatic = isStatic;
		return isStatic;
	}

	GameObject_::GameObject_(const GameObject_& other) :
		_name(other._name),
		_tag(other._tag),
		_isStatic(false)
	{
		//_transform = other._transform;
		//_components = other._components;
	}

	GameObject_& GameObject_::operator=(const GameObject_& other)
	{
		_name = other._name;
		_tag = other._tag;
		_isStatic = false;
		//_transform = other._transform;
		//_components = other._components;

		return *this;
	}
}
