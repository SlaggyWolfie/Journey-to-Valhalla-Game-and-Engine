#pragma once
#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <glm.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include "Object.hpp"
#include "GeneralHelpers.hpp"
#include "Core/GameLoop.hpp"
#include "Component.hpp"

namespace Engine
{
	namespace Core
	{
		class Component;
		class Transform;
		class GameLoop;

		class GameObject_ : Object
		{
		public:
			GameObject_(
				std::string name = "",
				std::string tag = "",
				const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
			virtual ~GameObject_();

			GameObject_(const GameObject_& other);
			GameObject_& operator= (const GameObject_& other);

			virtual void destroy() override;

			void setName(const std::string& name);
			std::string getName() const;

			void setTag(const std::string& tag);
			std::string getTag() const;

			//contains rotation, scale, position
			Transform* getTransform() const;

			bool isStatic() const;
			bool setStatic(bool isStatic = false);

			bool isActive() const;
			bool setActive(bool isActive = true);

			template <typename T>
			void addComponent();
			template <typename T>
			void removeComponent();
			template <typename T>
			bool containsComponent();

			void addComponent(Component* newComponent);
			void removeComponent(Component* component);
			bool containsComponent(Component* component);

			template <typename T>
			T* getComponent();
			template <typename T>
			T** getComponents();
			template <typename T>
			std::vector<T*> getComponentsList();

			int getComponentsCount() const;

		private:
			std::string _name = "";
			std::string _tag = "";
			Transform* _transform;

			GameLoop* _gameLoop = nullptr;
			GameLoop* getGameLoop();

			template <typename T>
			T* findComponent();

			int getIndex(Component* component);

			bool _isStatic = false;
			bool _isActive = true;

			std::vector<std::unique_ptr<Component>> _components;
		};


		template <typename T>
		T* GameObject_::getComponent()
		{
			return findComponent<T>();
		}

		template <typename T>
		T** GameObject_::getComponents()
		{
			std::vector<T*> components = getComponentsList<T>();
			const auto size = static_cast<unsigned int>(components.size());
			T** componentArray = new T*[size];
			std::copy(components.begin(), components.end(), componentArray);

			return componentArray;
		}

		template <typename T>
		std::vector<T*> GameObject_::getComponentsList()
		{
			//Confirm that type T is a component
			if (!std::is_base_of<Component, T>())
			{
				std::cout << std::string("Type T is not of type Component in getComponentsList<T>!") << std::endl;
				return nullptr;
			}

			std::vector<T*> components = new std::vector<T*>();

			//Search for a component that can be cast to T
			for (size_t i = 0; i < _components.size(); i++)
			{
				const auto comp = dynamic_cast<T*>(_components[i].get());
				if (comp != nullptr)
					components.push_back(comp);
			}

			//Return null if we can't find anything
			return components;
		}

		template <typename T>
		void GameObject_::addComponent()
		{
			const auto comp = findComponent<T>();

			if (comp != nullptr
				&& static_cast<Component*>(comp)->isUniquePerGameObject())
			{
				std::cout << "Attempting to add unique per game object component:" << std::endl;
				return;
			}

			//Might crash
			if (std::is_default_constructible<T>::value)
			{
				//_components.push_back(newComponent);
				_components.emplace_back(std::make_unique<T>());
				T* newComponent = static_cast<T*>((&*(_components.end() - 1))->get());
				newComponent->setGameObject(this);
				getGameLoop()->subscribe(newComponent);
			}
			else std::cout << "Failed to add component. Has no default constructor." << std::endl;
		}

		template <typename T>
		void GameObject_::removeComponent()
		{
			Component* comp = findComponent<T>();

			if (comp == nullptr) return;

			getGameLoop()->unsubscribe(comp);

			_components.erase(_components.begin() + getIndex(comp));
			comp->destroy();
		}

		template <typename T>
		bool GameObject_::containsComponent()
		{
			return findComponent<T>() != nullptr;
		}

		template <typename T>
		T* GameObject_::findComponent()
		{
			if (!std::is_base_of<Component, T>())
			{
				std::cout << "Type T is not of type Component in findComponent()" << std::endl;
				return nullptr;
			}

			for (auto & component : _components)
			{
				T* cast_component = dynamic_cast<T*>(component.get());

				//std::cout << "I am copying: " + std::string(cast_component != component.get() ? "true." : "false.") << std::endl;
				if (cast_component != nullptr)
					return cast_component;
			}
			//std::cout << "boi_______" << std::endl;

			return nullptr;
		}
	}
}

#endif //GAME_OBJECT_HPP