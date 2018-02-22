#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"

namespace Engine
{
	namespace Core
	{
		Camera_* Camera_::_main = nullptr;

		Camera_::Camera_() :
			_fov(45), _nearPlaneDistance(0.3f), _farPlaneDistance(1000), _projectionMode(Perspective)
		{
		}

		Camera_::Camera_(const ProjectionMode projectionMode) :
			_fov(45), _nearPlaneDistance(0.3f), _farPlaneDistance(1000), _projectionMode(projectionMode)
		{
		}

		Camera_* Camera_::getMainCamera()
		{
			return _main;
		}

		glm::mat4 Camera_::getViewMatrix() const
		{
			return glm::inverse(getGameObject()->getTransform()->getMatrix4X4());
		}

		glm::mat4 Camera_::getProjectionMatrix() const
		{
			sf::RenderWindow* window = ServiceLocator::instance()->getService<Game>()->getWindow();
			switch (_projectionMode)
			{
			case Perspective:
				return glm::perspective(
					glm::radians(_fov),
					window->getSize().x / static_cast<float>(window->getSize().y),
					_nearPlaneDistance, _farPlaneDistance);
			case Orthographic:
				return glm::ortho(
					0.0f, static_cast<float>(window->getSize().x),
					0.0f, static_cast<float>(window->getSize().y),
					_nearPlaneDistance, _farPlaneDistance);
			default:
				return glm::mat4();
			}
		}

		glm::vec3 Camera_::getPosition() const
		{
			return getGameObject()->getTransform()->getPosition();
		}

		void Camera_::setFOV(const float fov)
		{
			_fov = fov;
		}

		void Camera_::setNearPlaneDistance(const float near)
		{
			_nearPlaneDistance = near;
		}

		void Camera_::setFarPlaneDistance(const float far)
		{
			_farPlaneDistance = far;
		}

		void Camera_::setProjectionMode(const ProjectionMode projectionMode)
		{
			_projectionMode = projectionMode;
		}

		float Camera_::getFOV() const
		{
			return _fov;
		}

		float Camera_::getNearPlaneDistance() const
		{
			return _nearPlaneDistance;
		}

		float Camera_::getFarPlaneDistance() const
		{
			return _farPlaneDistance;
		}

		ProjectionMode Camera_::getProjectionMode() const
		{
			return _projectionMode;
		}

		void Camera_::awake()
		{
			_main = this;
		}
	}
}
