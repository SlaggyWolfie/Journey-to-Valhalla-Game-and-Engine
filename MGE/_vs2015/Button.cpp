#include "Button.hpp"
#include <iostream>
#include <GL/glew.h>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"
#include "Core\GameLoop.hpp"

namespace Engine
{
	namespace UI
	{
		std::map<std::string, std::vector<Button*>> Button::menus;

		Button::Button(const bool rendering) : ComponentUI(rendering)
		{

		}

		Button::Button(sf::RenderWindow* window, const bool rendering) : ComponentUI(window, rendering)
		{
		}

		void Button::draw()
		{
			switch (_status)
			{
			case Clicked:
				if (_clickingSpriteLoaded)
				{
					getWindow()->draw(_clickSprite);
					break;
				}
			case Hovering:
				if (_hoveringSpriteLoaded)
				{
					getWindow()->draw(_hoverSprite);
					break;
				}
			case Normal:
				getWindow()->draw(_normalSprite);
				break;
			default:
				break;
			}
		}

		void Button::lateUpdate()
		{
		}
		void Button::update()
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition();

			sf::Vector2i btnPos = (sf::Vector2i) _normalSprite.getPosition();
			sf::Vector2i btnSize = (sf::Vector2i) _normalSprite.getTexture()->getSize();
			bool xCheck = mousePos.x > btnPos.x - btnSize.x && mousePos.x > btnPos.x + btnSize.x;
			bool yCheck = mousePos.y > btnPos.y - btnSize.y && mousePos.y > btnPos.y + btnSize.y;

			bool mouseClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

			if (xCheck && yCheck && mouseClick)
			{
				//scene load scene1
				std::cout << "click event" << std::endl;
				onClick();
				return;
			}

			if (xCheck && yCheck)
			{
				onHover();
			}
			else
			{
				onNotHover();
			}
		}

		void Button::onHover()
		{
			if (_status == Hovering) return;

			_status = Hovering;
			//if (!_hoveringSpriteLoaded)
			//{
			//	setDrawable(&_normalSprite);
			//	return;
			//}
			//setDrawable(&_hoverSprite);
		}

		void Button::onClick()
		{
			if (_status == Clicked) return;
			_status = Clicked;
			//if (!_clickingSpriteLoaded)
			//{
			//	if (!_hoveringSpriteLoaded) {

			//		setDrawable(&_normalSprite);
			//		return;
			//	}
			//	setDrawable(&_hoverSprite);
			//	return;
			//}
			//setDrawable(&_clickSprite);
		}

		void Button::onNotHover()
		{
			if (_status == Normal) return;
			_status = Normal;

			//setDrawable(&_normalSprite);
		}

		void Button::loadSprite(const sf::Sprite & sprite, ButtonStatus status)
		{
			switch (status)
			{
			default:
				std::cout << "Unknown type of sprite button default to normal." << std::endl;
			case Normal:
				_normalSprite = sprite;
				_normalSpriteLoaded = true;
				break;
			case Hovering:
				_hoverSprite = sprite;
				_hoveringSpriteLoaded = true;
				break;
			case Clicked:
				_clickSprite = sprite;
				_clickingSpriteLoaded = true;
				break;
			}
		}

		void Button::loadSprite(const std::string & path,float x,float y, ButtonStatus status)
		{
			sf::Sprite* sprite = new sf::Sprite();
			sf::Texture* texture = new sf::Texture();
			if (!texture->loadFromFile(path))
			{
				std::cout << "Texture not loaded." << std::endl;
				return;
			}
			sprite->setTexture(*texture);
			sprite->setPosition(sf::Vector2f(x, y));
			loadSprite(*sprite, status);
		}

		sf::Sprite & Button::getSprite(ButtonStatus status)
		{

			switch (status)
			{
			default: std::cout << "Unknown type of sprite button default to normal." << std::endl;
			case Normal: return _normalSprite;
			case Hovering: return _hoverSprite;
			case Clicked: return _clickSprite;
			}
		}

		void Button::createMenu()
		{
			//_playButton = new sf::Buttonure();
			//_optionsButton = new sf::Buttonure();
			//_creditsButton = new sf::Buttonure();
			//_exitButton = new sf::Buttonure();

			//_playButton->loadFromFile("");

			///*_playSpr =  sf::Button(_playButton);
			//_optionsSpr = sf::Button(_optionsButton);
			//_creditsSpr = sf::Button(_creditsButton);
			//_exitSpr = sf::Button(_exitButton);*/

			//_playSpr.setPosition(sf::Vector2f(50, 100));
		}

		void Button::SetEvent(std::string event)
		{
		}

		void Button::OneShotHint(std::string hint)
		{
			using namespace Engine::Utility;
			Time::now();
		}
	}
}
