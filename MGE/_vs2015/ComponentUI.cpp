#include "ComponentUI.hpp"
#include "ServiceLocator.hpp"
#include "RenderManager.hpp"
#include "Core/Game.hpp"

ComponentUI::ComponentUI(const bool rendering) : _mainDrawable(nullptr), _window(nullptr)
{
	Engine::ServiceLocator::instance()->getService<Engine::Core::GameLoop>()->subscribe(this);
	if (rendering)
		Engine::ServiceLocator::instance()->getService<Engine::Rendering::RenderManager>()->addUI(this);
}

ComponentUI::ComponentUI(sf::RenderWindow * window, const bool rendering) : _mainDrawable(nullptr), _window(window)
{
	Engine::ServiceLocator::instance()->getService<Engine::Core::GameLoop>()->subscribe(this);
	if (rendering)
		Engine::ServiceLocator::instance()->getService<Engine::Rendering::RenderManager>()->addUI(this);
}

ComponentUI::~ComponentUI()
{
	delete _mainDrawable;
	Engine::ServiceLocator::instance()->getService<Engine::Rendering::RenderManager>()->removeUI(this);
}

//void ComponentUI::draw()
//{
//	_window->draw(*_mainDrawable);
//}

void ComponentUI::setWindow(sf::RenderWindow * window)
{
	_window = window;
}

void ComponentUI::setDrawable(sf::Drawable * drawable)
{
	_mainDrawable = drawable;
}

sf::Drawable * ComponentUI::getDrawable() const
{
	return _mainDrawable;
}

sf::RenderWindow * ComponentUI::getWindow()
{
	if (!_window) _window = Engine::ServiceLocator::instance()->getService < Engine::Game >()->getWindow();
	return _window;
}
