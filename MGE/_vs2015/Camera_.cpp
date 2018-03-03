#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"

namespace Engine
{
	namespace Core
	{
		Camera_* Camera_::_main = nullptr;

		Camera_::Camera_() :
			_fov(45), _nearPlaneDistance(0.3f), _farPlaneDistance(10000), _projectionMode(Perspective)
		{
		}

		Camera_::Camera_(const ProjectionMode projectionMode) :
			_fov(45), _nearPlaneDistance(0.3f), _farPlaneDistance(10000), _projectionMode(projectionMode)
		{
		}

		Camera_* Camera_::getMainCamera()
		{
			return _main;
		}

		void Camera_::setMainCamera(Camera_* camera)
		{
			_main = camera;
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
			//_main = this;
		}

		void Camera_::update()
		{
			float speed = 3.0f;
			float turnSpeed = 10.0f;
			Transform* transform = getGameObject()->getTransform();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) transform->translate(transform->forward() * speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) transform->translate(transform->forward() * -speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) transform->translate(transform->right() * -speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) transform->translate(transform->right() * speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) transform->translate(transform->up() * speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) transform->translate(transform->up() * -speed);

			/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) transform->rotate(transform->right(), glm::radians(turnSpeed) * Utility::Time::deltaTime());
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) transform->rotate(transform->right(), glm::radians(-turnSpeed) * Utility::Time::deltaTime());
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) transform->rotate(transform->up(), glm::radians(turnSpeed) * Utility::Time::deltaTime());
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) transform->rotate(transform->up(), glm::radians(-turnSpeed) * Utility::Time::deltaTime());*/

			//std::cout << "Position: " + glm::to_string(transform->getPosition()) << std::endl;
			//std::cout << "Rotation: " + glm::to_string(glm::degrees(glm::eulerAngles(transform->getRotation()))) << std::endl;
			//std::cout << "Scale: " + glm::to_string(transform->getScale()) << std::endl;
			//std::cout << "Forward: " + glm::to_string(transform->forward()) << std::endl;
			//std::cout << "Right: " + glm::to_string(transform->right()) << std::endl;
			//std::cout << "Up: " + glm::to_string(transform->up()) << std::endl << std::endl;

			//std::cout << "Local Matrix: " << std::endl << glm::to_string(transform->getLocalMatrix4X4()) << std::endl << std::endl;
			//std::cout << "World Matrix: " <<std::endl << glm::to_string(transform->getMatrix4X4()) << std::endl << std::endl;
		}
	}
}
