#pragma once
#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Object.hpp"

namespace Engine
{
	namespace Core
	{
		class GameObject_;

		class Component : Object
		{
			friend class GameObject_;

		public:
			GameObject_ * getGameObject() const;
			Component();
			virtual ~Component();
			Component(const Component&);
			Component& operator=(const Component&);
			virtual void destroy() override;
		protected:
			virtual void start() {};
			virtual void awake() {};
			virtual void update() {};
			virtual void fixedUpdate() {};
			virtual void lateUpdate() {};
			virtual void onValidate() {};

			void setGameObject(GameObject_* gameObject);
			virtual bool isUniquePerGameObject();
		private:
			GameObject_ * _gameObject;
		};
	}
}

#endif //COMPONENT_HPP