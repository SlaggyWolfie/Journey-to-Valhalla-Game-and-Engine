#include "Button.hpp"
#include <iostream>
#include <GL/glew.h>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"
#include "Core\GameLoop.hpp"
#include "SceneManager.hpp"
#include "InputHandler.hpp"

namespace Engine
{
	namespace UI
	{
		std::map<std::string, std::vector<Button*>> Button::menus;
		bool Button::_initialSetup = false;
		sf::Texture Button::textureBG;
		sf::Sprite Button::pauseMenuBG;
		bool Button::DrawPauseMenu = true;

		Button::Button(const bool rendering) : ComponentUI(rendering)
		{
			functions["LevelMenu"] = ButtonFunctionality::LevelMenu;
			functions["MainMenu"] = ButtonFunctionality::MainMenu;
			functions["OpenLevel"] = ButtonFunctionality::OpenLevel;
			functions["Exit"] = ButtonFunctionality::Exit;
			functions["Options"] = ButtonFunctionality::Options;
			functions["Credits"] = ButtonFunctionality::Credits;

		}

		Button::Button(sf::RenderWindow* window, const bool rendering) : ComponentUI(window, rendering)
		{
		}

		void Button::start()
		{
			if (!_initialSetup)
			{
				textureBG.loadFromFile("container.png");
				pauseMenuBG.setTexture(textureBG);
				disableAllMenus();
				enableMenu("MainMenu");
				_initialSetup = true;
			}
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
			delay--;
			sf::Vector2i mousePos = sf::Mouse::getPosition(*getWindow());
			//250x60
			sf::Vector2i btnPos = (sf::Vector2i) _normalSprite.getPosition();
			sf::Vector2i btnSize = (sf::Vector2i) _normalSprite.getTexture()->getSize();
			//bool xCheck = mousePos.x > btnPos.x;
			//bool yCheck = mousePos.y > btnPos.y  && mousePos.y > btnPos.y + 60;
			//std::cout << btnPos.x << " " << btnPos.y << " " << _function << std::endl;
			bool xCheck = mousePos.x > btnPos.x + 40 && mousePos.x < btnPos.x + btnSize.x - 40;
			bool yCheck = mousePos.y > btnPos.y + 5 && mousePos.y < btnPos.y + btnSize.y - 5;
			//t++;


			using namespace Engine::Utility;
			//not recorded mouse click
			if (_function == LevelMenu)
				mouseClick = InputHandler::keyUp(sf::Mouse::Button::Left);
			else
				mouseClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

			//if (mouseClick && t==0)
			t -= Time::deltaTime();
			if (mouseClick) std::cout << std::to_string(xCheck) << " " << std::to_string(yCheck) << std::endl;
			if (xCheck && yCheck && mouseClick)
			{
				delay = 1000;
				t = 0.5f;
				std::cout << "click event" << std::endl;
				onClick();
				mouseClick = false;
				//std::cout << btnPos.x << " " << mousePos.x << std::endl;
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

			//std::cout << "ia tut" << std::endl;
			switch (_function)
			{
			case MainMenu:
				disableAllMenus();
				enableMenu("MainMenu");
				//wait fo 3 sec
				//reset mouse recorded
				break;
			case LevelMenu:
				disableAllMenus();

				enableMenu("LevelMenu");
				//reset mouse recorded

				break;
			case OpenLevel:
			{
				disableAllMenus();
				SceneManager* scene_m = ServiceLocator::instance()->getService<SceneManager>();
				scene_m->loadScene(scene_m->getLevel(_levelToOpen));
				//ServiceLocator::instance()->getService<SceneManager>()->loadScene(std::string("Level_") + std::to_string(_levelToOpen) + ".json");
				//show loading screen
				//_levelToOpen
				//reset mouse recorded

				break;
			}
			case Exit:
				ServiceLocator::instance()->getService<Game>()->exit();
				break;
			case Options:
				disableAllMenus();
				enableMenu("Options");
				break;
			case Credits:
				disableAllMenus();
				enableMenu("Credits");

				break;
			}
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

		void Button::disableAllMenus(std::string exceptionMenu)
		{
			//Disable all
			for (auto& menu : menus)
				//if (menu.first != exceptionMenu)
				for (auto button : menu.second)
					button->setEnabled(false);
		}

		void Button::enableMenu(std::string menu)
		{
			if (menus.find(menu) != menus.end())
				for (auto button : menus[menu])
					button->setEnabled(true);
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

		void Button::loadSprite(const std::string & path, float x, float y, ButtonStatus status)
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

		void Button::SetEvent(std::string functionString)
		{
			//didnt test yet
			if (functionString.substr(0, functionString.length() - 1) == "OpenLevel")
			{
				//Hello C
				auto str = functionString[functionString.length() - 1];
				sscanf(&str, "%i", &_levelToOpen);
				//_levelToOpen = functionString[functionString.length() - 1];
				_function = functions.at(functionString.substr(0, functionString.length() - 1));
			}
			else
			{
				//std::cout << functionString << std::endl;
				_function = functions.at(functionString);
			}

		}

		void Button::OneShotHint(std::string hint)
		{
			using namespace Engine::Utility;
			Time::now();
		}
		void Button::TagglePauseMenu()
		{

		}
	}
}
