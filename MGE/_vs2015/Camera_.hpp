#pragma once
#ifndef CAMERA__HPP
#define CAMERA__HPP
#include "Component.hpp"
#include <glm/glm.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace Engine
{
	namespace Core
	{
		enum ProjectionMode { Perspective, Orthographic };

		class Camera_ : public Component
		{
		public:
			Camera_();
			Camera_(ProjectionMode projectionMode);
			virtual ~Camera_() = default;
			Camera_(const Camera_& other) = default;
			Camera_& operator=(const Camera_& other) = default;

			static Camera_* getMainCamera();
			static void setMainCamera(Camera_* camera);
			glm::mat4 getViewMatrix() const;
			glm::mat4 getProjectionMatrix() const;
			glm::vec3 getPosition() const;

			void setFOV(float fov);
			void setNearPlaneDistance(float near);
			void setFarPlaneDistance(float far);
			void setProjectionMode(ProjectionMode projectionMode);

			float getFOV() const;
			float getNearPlaneDistance() const;
			float getFarPlaneDistance() const;
			ProjectionMode getProjectionMode() const;

		protected:
			void awake() override;
			void update() override;
		private:
			static Camera_* _main;
			float _fov;
			float _nearPlaneDistance;
			float _farPlaneDistance;
			ProjectionMode _projectionMode;

			bool _isButtonHeld = false;
			sf::Vector2i _heldPosition = sf::Vector2i();

			void cameraControl();
			static glm::vec3 moveTowards(glm::vec3 current, glm::vec3 target, float maxDelta);
			static sf::Vector2i moveTowards(sf::Vector2i current, sf::Vector2i target, int maxDelta);
			static sf::Vector2i lerp(sf::Vector2i current, sf::Vector2i target, float t);
		};
	}
}

#endif
