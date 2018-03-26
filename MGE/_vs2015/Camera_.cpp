#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
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
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				Transform* transform = getGameObject()->getTransform();
				transform->setRotation(glm::quat(glm::vec3(0, 0, 0)));
				transform->setPosition(glm::vec3(0, 0, 0));
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				if (!_isButtonHeld) _heldPosition = sf::Mouse::getPosition();
				_isButtonHeld = true;
				//return;
			}
			else _isButtonHeld = false;

			if (_isButtonHeld)
				cameraControl();
		}

		void Camera_::cameraControl()
		{
			const sf::Vector2i mousePosition = sf::Mouse::getPosition();

			float moveSpeed = 113.0f
				* Utility::Time::deltaTime()
				;
			float turnSpeed = 100.0f
				* Utility::Time::deltaTime()
				;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
			{
				moveSpeed *= 5;
				turnSpeed *= 5;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			{
				moveSpeed /= 5;
				turnSpeed /= 5;
			}

			Transform* transform = getGameObject()->getTransform();

			//if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			//{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) transform->translate(transform->forward() * moveSpeed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) transform->translate(transform->forward() * -moveSpeed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) transform->translate(transform->right() * -moveSpeed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) transform->translate(transform->right() * moveSpeed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) transform->translate(transform->up() * moveSpeed);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) transform->translate(transform->up() * -moveSpeed);
			//}
			//else
			//{
			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) transform->rotate(transform->right(), glm::radians(turnSpeed));
			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) transform->rotate(transform->right(), glm::radians(-turnSpeed));
			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) transform->rotate(transform->up(), glm::radians(turnSpeed));
			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) transform->rotate(transform->up(), glm::radians(-turnSpeed));

			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) transform->rotate(transform->forward(), glm::radians(-turnSpeed));
			//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) transform->rotate(transform->forward(), glm::radians(turnSpeed));
			//}

			const sf::Vector2i deltaScreenSpace = mousePosition - _heldPosition;
			//deltaScreenSpace.
			if (deltaScreenSpace.x != 0 || deltaScreenSpace.y != 0)
			{
				sf::RenderWindow* window = ServiceLocator::instance()->getService<Game>()->getWindow();
				const float screenWidth = window->getSize().x;
				const float screenHeight = window->getSize().y;
				const float x = glm::radians(-deltaScreenSpace.y / screenHeight * turnSpeed);
				const float y = glm::radians(-deltaScreenSpace.x / screenWidth * turnSpeed);

				//std::cout << "Mouse Delta: " + std::to_string(x) + ", " + std::to_string(z) << std::endl;
				const glm::vec3 delta = glm::vec3(x, y, 0);
				//transform->rotate(delta, true, World);
				transform->rotate(glm::vec3(0, 1, 0), y, World);
				transform->rotate(transform->right(), x);

				//_heldPosition = moveTowards(_heldPosition, mousePosition,
				//	static_cast<int>(turnSpeed) * Utility::Time::deltaTime());
				_heldPosition = lerp(_heldPosition, mousePosition, 0.005f);
				//std::hypot(screenWidth, screenHeight)) * turnSpeed * 0.5f
				//const glm::vec3 delta = glm::normalize(glm::vec3(deltaScreenSpace.x, deltaScreenSpace.y, 0));
				//const glm::vec3 rotationAxis = glm::normalize(glm::cross(transform->forward(), delta));
				//transform->rotate(rotationAxis, glm::radians(turnSpeed), World);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) transform->rotate(transform->right(), glm::radians(turnSpeed)
				//, Local
			);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) transform->rotate(transform->right(), glm::radians(-turnSpeed)
				//, Local
			);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) transform->rotate(transform->up(), glm::radians(turnSpeed)
				//, Local
			);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) transform->rotate(transform->up(), glm::radians(-turnSpeed)
				//, Local
			);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)) transform->rotate(transform->forward(), glm::radians(-turnSpeed)
				//, Local
			);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)) transform->rotate(transform->forward(), glm::radians(+turnSpeed)
				//, Local
			);

			//std::cout << "Held Position: " + std::to_string(_heldPosition.x) + ", " + std::to_string(_heldPosition.y) << std::endl;
			//std::cout << "Mouse Position: " + std::to_string(mousePosition.x) + ", " + std::to_string(mousePosition.y) << std::endl;
			//std::cout << "Position: " + glm::to_string(transform->getPosition()) << std::endl;
			//std::cout << "Rotation: " + glm::to_string(glm::degrees(glm::eulerAngles(transform->getRotation()))) << std::endl;
			//std::cout << "Scale: " + glm::to_string(transform->getScale()) << std::endl;
			//std::cout << "Forward: " + glm::to_string(transform->forward()) << std::endl;
			//std::cout << "Right: " + glm::to_string(transform->right()) << std::endl;
			//std::cout << "Up: " + glm::to_string(transform->up()) << std::endl;
			//std::cout << "Quaternion: " + glm::to_string(transform->getRotation() )<<std::endl;
			//std::cout << "Quat length: " + std::to_string(glm::length(transform->getRotation())) << std::endl << std::endl;
			//std::
			//std::cout << "Local Matrix: " << std::endl << glm::to_string(transform->getLocalMatrix4X4()) << std::endl << std::endl;
			//std::cout << "World Matrix: " <<std::endl << glm::to_string(transform->getMatrix4X4()) << std::endl << std::endl;
		}

		glm::vec3 Camera_::moveTowards(const glm::vec3 current, const glm::vec3 target, const float maxDelta)
		{
			const glm::vec3 delta = target - current;
			const glm::vec3 movementDelta = glm::length2(delta) > maxDelta * maxDelta ? glm::normalize(delta) * maxDelta : delta;
			return current + movementDelta;
		}

		sf::Vector2i Camera_::moveTowards(const sf::Vector2i current, const sf::Vector2i target, const int maxDelta)
		{
			const sf::Vector2i sf_delta = target - current;
			const glm::vec2 delta = glm::vec2(sf_delta.x, sf_delta.y);
			const glm::ivec2 movementDelta = glm::length2(delta) > maxDelta * maxDelta ? glm::normalize(delta) * maxDelta : delta;
			const sf::Vector2i sf_movementDelta = sf::Vector2i(movementDelta.x, movementDelta.y);
			return current + sf_movementDelta;
		}

		sf::Vector2i Camera_::lerp(const sf::Vector2i current, const sf::Vector2i target, const float t)
		{
			const glm::vec2 glm_current = glm::vec2(current.x, current.y);
			const glm::vec2 glm_target = glm::vec2(target.x, target.y);
			const glm::ivec2 glm_lerp = glm::lerp(glm_current, glm_target, t);
			return sf::Vector2i(glm_lerp.x, glm_lerp.y);
		}
	}
}
