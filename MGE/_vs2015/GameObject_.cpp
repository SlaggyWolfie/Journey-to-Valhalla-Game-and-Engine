#include "GameObject_.hpp"
#include <utility>
//#include "GameObject.hpp"
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
			for (size_t i = 0; i < _components.size(); i++)
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
			_isActive = other._isActive;
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
