#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "ComponentUI.hpp"
#include <map>
#include <vector>

namespace Engine
{
	namespace UI
	{
		enum ButtonStatus { Normal, Hovering, Clicked };
		enum ButtonFunctionality { LevelMenu, MainMenu, OpenLevel, Exit, Options, Credits,BackToGame};

		class Button : public ComponentUI
		{
		public:
			static std::map<std::string, std::vector<Button*>> menus; 
			//static std::vector<Button*> allButtons;
			static sf::Sprite pauseMenuBG;
			static sf::Texture textureBG;
			static bool DrawPauseMenu;
			static void  TogglePauseMenu();
			static void disableAllMenus(std::string exceptionMenu = "");
			static void enableMenu(std::string menu);
			//static 

			Button(bool rendering = true);
			Button(sf::RenderWindow* window, bool rendering = true);
			virtual ~Button() = default;
			Button(const Button& other) = default;
			Button& operator=(const Button& other) = default;

			void loadSprite(const sf::Sprite& sprite, ButtonStatus status = Normal);
			void loadSprite(const std::string& path, float x = 0, float y = 0, ButtonStatus status = Normal);
			sf::Sprite& getSprite(ButtonStatus status = Normal);
			void createMenu();
			void SetEvent(std::string event);
			void OneShotHint(std::string hint);
			bool isClicked();
			bool isHovering();
			void OnReleaseClick();
			void start() override;
			void draw() override;
			void lateUpdate() override;
			void update() override;
		private:
			static sf::Vector2f moveTowards(sf::Vector2f current, sf::Vector2f target, float delta);

			std::map<std::string, ButtonFunctionality> functions;
			bool _normalSpriteLoaded = false;
			bool _hoveringSpriteLoaded = false;
			bool _clickingSpriteLoaded = false;
			sf::Sprite _normalSprite;
			sf::Sprite _hoverSprite;
			sf::Sprite _clickSprite;

			float t = 0;
			float delay= 0;
			int _levelToOpen = 0;
			bool _isHovering;
			bool _isClicked;
			ButtonFunctionality _function = MainMenu;
			ButtonStatus _status = Normal;
			void onHover();
			void onClick();
			void onNotHover();
			bool _recorder = false;
			bool mouseClick = false;
			static bool _initialSetup;
		};
	}
}
