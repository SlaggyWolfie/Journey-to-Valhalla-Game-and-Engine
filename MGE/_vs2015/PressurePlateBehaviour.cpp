#include "PressurePlateBehaviour.h"
#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"
using namespace Engine::Core;
using namespace Engine;


PressurePlateBehaviour::PressurePlateBehaviour()
{

	
}

void PressurePlateBehaviour::update()
{
	TogglePositions();
	CheckCollision();
}

bool PressurePlateBehaviour::IsPressed()
{
	return _isPressed;
}

void PressurePlateBehaviour::SetPressed(bool value)
{
	_isPressed = value;
}

void PressurePlateBehaviour::CheckCollision()
{
	std::vector<collider*> colList= ServiceLocator::instance()->getService<ColliderManager>()->
		CheckBoxCollision(this->getGameObject()->getComponent<collider>());
	/*std::cout << colList.size()<<" size " << std::endl;*/
	/*std::cout << this->getGameObject()->getComponent<collider>()->GetPos() << " pos " << std::endl;*/

	for (int i = 0; i < colList.size(); i++)
	{	
		//std::cout << colList.size() << " "<<"size" << std::endl;
		/*if (colList[i]->getGameObject()->getName().find(
			colList[i]->getGameObject()->getName().begin(), colList[i]->getGameObject()->getName().end(), "crate")
			!= std::string::npos)
		{
			std::cout << "I should be pressed" << std::endl;
			this->SetPressed(true);
			return;
		}*/
	}
		//else
		{
			this->SetPressed(false);
		}
}

void PressurePlateBehaviour::start()
{
	Transform* transform = getGameObject()->getTransform();
	_notActivatedPos = transform->getPosition();
	_activatedPos = transform->getPosition() + glm::vec3(0, -9, 0);
}

void PressurePlateBehaviour::TogglePositions()
{
	if (_isPressed)
	{
		this->getGameObject()->getTransform()->setPosition(lerp(this->getGameObject()->getTransform()->getPosition(), _activatedPos, 0.1f));
	}
	else
	{
		this->getGameObject()->getTransform()->setPosition(lerp(this->getGameObject()->getTransform()->getPosition(), _notActivatedPos, 0.1f));
	}
}
void PressurePlateBehaviour::destroy()
{
	delete this;
}

bool PressurePlateBehaviour::isUniquePerGameObject()
{
	return true;
}
