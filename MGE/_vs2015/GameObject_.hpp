#pragma once
#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <glm.hpp>
#include <vector>
#include <memory>
#include "Object.hpp"

namespace Engine
{
	namespace Core
	{
		class Component;
		class Transform;

		class GameObject_ : Object
		{
		public:
			GameObject_(
				std::string name = nullptr,
				std::string tag = nullptr,
				const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
			virtual ~GameObject_();

			GameObject_(const GameObject_& other);
			GameObject_& operator= (const GameObject_& other);

			void destroy();

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

			void addComponent(Component* component);
			void removeComponent(Component* component);

			template <typename T>
			T* getComponent();
			template <typename T>
			T** getComponents();
			template <typename T>
			std::vector<T*> getComponents();

		private:
			std::string _name;
			std::string _tag;
			std::unique_ptr<Transform> _transform;

			bool _isStatic;
			bool _isActive;

			std::vector<std::unique_ptr<Component>> _components;
		};
	}
}

#endif //GAME_OBJECT_HPP