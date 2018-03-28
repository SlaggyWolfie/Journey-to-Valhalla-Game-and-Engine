#pragma once
#ifndef COMPONENT_UI_HPP
#define COMPONENT_UI_HPP
#include <SFML/Graphics.hpp>
#include "Component.hpp"

class ComponentUI : public Engine::Core::Component
{
public:
	explicit ComponentUI(bool rendering = true);
	ComponentUI(sf::RenderWindow* window, bool rendering);
	virtual ~ComponentUI();

	virtual void draw() = 0;
	void setWindow(sf::RenderWindow* window);
	void setDrawable(sf::Drawable* drawable);
	sf::Drawable* getDrawable() const;
	sf::RenderWindow* getWindow();
protected:
	sf::Drawable* _mainDrawable = nullptr;
	sf::RenderWindow* _window = nullptr;
};

#endif