#include "GameObject_.hpp"
#include <utility>
#include <algorithm>
#include "Transform.hpp"
#include "Component.hpp"

namespace Engine
{
	namespace Core
	{
		/**
		 * \brief
		 * \param name Game Object's name.
		 * \param tag Game Object's tag.
		 * \param position Game Object's starting position.
		 */
		GameObject_::GameObject_(std::string name, std::string tag, const glm::vec3 & position)
			: _name(std::move(name)), _tag(std::move(tag)), _isStatic(false), _isActive(true)
		{
			_transform = std::make_unique<Transform>();
			_components = std::vector<std::unique_ptr<Component>>();
			_components.push_back(std::unique_ptr<Component>(_transform.get()));
		}

		GameObject_::~GameObject_()
		{
			//Search for a component that can be cast to T
			for (std::unique_ptr<Component> & component : _components)
				component->destroy();
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
		T** GameObject_::getComponents()
		{
			std::vector<T*> components = getComponents<T>();
			const int size = static_cast<unsigned int>(components.size());
			T** componentArray = new T*[size];
			std::copy(components.begin(), components.end(), componentArray);

			return componentArray;
		}

		template <typename T>
		std::vector<T*> GameObject_::getComponents()
		{
			//Confirm that type T is a component
			if (!std::is_base_of<Engine::Component, T>())
			{
				std::cout << std::string("Type T is not of type Component in getComponent<T>!") << std::endl;
				return nullptr;
			}

			std::vector<T*> components = new std::vector<T*>();

			//Search for a component that can be cast to T
			for (int i = 0; i < _components.size(); i++)
			{
				if (dynamic_cast<T>(_components[i].get()) != nullptr)
					components.push_back(_components[i].get());
			}

			//Return null if we can't find anything
			return components;
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
				std::unique_ptr<Component> newComponent = std::unique_ptr<Component>(derivedComponent);
				newComponent.get()->setGameObject(this);
				_components.push_back(newComponent);
			}
			else std::cout << "Failed to add component. Has no default constructor." << std::endl;
		}

		template <typename T>
		void GameObject_::removeComponent()
		{
			//Confirm that type T is a component
			if (!std::is_base_of<Component, T>())
			{
				std::cout << std::string("Type T is not of type Component in removeComponent<T>!") << std::endl;
				return;
			}

			//Search for a component that can be cast to T
			for (int i = 0; i < _components.size(); i++)
			{
				Component* component = _components[i].get();
				T* derivedComponent = dynamic_cast<T>(component);
				if (derivedComponent != nullptr)
				{
					_components.erase(
						std::remove(
							_components.begin(), _components.end(), component
						), _components.end());

					_components[i].reset();
					//delete component;
					return;
				}
			}
		}
		template <typename T>
		bool GameObject_::containsComponent()
		{
			//Confirm that type T is a component
			if (!std::is_base_of<Component, T>())
			{
				std::cout << std::string("Type T is not of type Component in removeComponent<T>!") << std::endl;
				return false;
			}

			//Search for a component that can be cast to T
			for (int i = 0; i < _components.size(); i++)
			{
				Component* component = _components[i].get();
				T* derivedComponent = dynamic_cast<T>(component);
				if (derivedComponent != nullptr) return true;
			}

			return false;
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

		bool GameObject_::isActive() const
		{
			return _isActive;
		}

		bool GameObject_::setActive(const bool isActive)
		{
			_isActive = isActive;
			return isActive;
		}

		void GameObject_::addComponent(Component* component)
		{
			component->setGameObject(this);
			_components.push_back(std::unique_ptr<Component>(component));
		}

		void GameObject_::removeComponent(Component* component)
		{
			for (auto i = 0; i < _components.size(); i++)
			{
				Component* iteratedComponent = _components[i].get();

				if (component == iteratedComponent)
				{
					_components.erase(
						std::remove(
							_components.begin(), _components.end(), _components[i]
						), _components.end());
					_components[i].reset();
					return;
				}
			}
		}

		bool GameObject_::containsComponent(Component* component)
		{
			//Search for a component that can be cast to T
			for (auto & i_Component : _components)
			{
				Component* iteratedComponent = i_Component.get();
				if (iteratedComponent == component) return true;
			}

			return false;
		}

		GameObject_::GameObject_(const GameObject_& other) :
			_name(other._name),
			_tag(other._tag),
			_isStatic(other._isStatic),
			_isActive(other._isActive)
		{
			//_transform = other._transform;
			//_components = other._components;
		}

		GameObject_& GameObject_::operator=(const GameObject_& other)
		{
			_name = other._name;
			_tag = other._tag;
			_isStatic = other._isStatic;
			_isStatic = other._isActive;
			//_transform = other._transform;
			//_components = other._components;

			return *this;
		}

		void GameObject_::destroy()
		{
			delete this;
		}
	}
}
