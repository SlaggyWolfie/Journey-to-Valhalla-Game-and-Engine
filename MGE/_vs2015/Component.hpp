#pragma once
#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "Object.hpp"
#include "RenderManager.hpp"

namespace Engine
{
	//namespace Rendering { class RenderManager; }
	namespace Core
	{
		class GameObject_;
		class Transform;

		class Component : public Object
		{
			friend class Engine::Rendering::RenderManager;
			friend class GameObject_;
			friend class GameLoop;
			//friend class Engine::Rendering::RenderManager;
		public:
			GameObject_ * getGameObject() const;
			Transform* getTransform() const;
			Component();
			virtual ~Component();
			Component(const Component&);
			Component& operator=(const Component&);

			bool isEnabled() const;
			void setEnabled(bool enable);

			void destroy() override;
		protected:
			virtual void start() {};
			virtual void awake() {};
			virtual void prewake() {};
			virtual void update() {};
			virtual void fixedUpdate() {};
			virtual void lateUpdate() {};
			virtual void onValidate() {};
			virtual void renderDebug() {};

			void setGameObject(GameObject_* gameObject);
			virtual bool isUniquePerGameObject();
		private:
			GameObject_ * _gameObject;
			bool _enabled;
			bool _started;
		};
	}
}

#endif //COMPONENT_HPP