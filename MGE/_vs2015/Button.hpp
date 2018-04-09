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
		enum ButtonFunctionality { LevelMenu, MainMenu, OpenLevel, Exit, Options, Credits };

		class Button : public ComponentUI
		{
		public:
			static std::map<std::string, std::vector<Button*>> menus;
			//static 

			Button(bool rendering = true);
			Button(sf::RenderWindow* window, bool rendering = true);
			virtual ~Button() = default;
			Button(const Button& other) = default;
			Button& operator=(const Button& other) = default;

			void loadSprite(const sf::Sprite& texture, ButtonStatus status = Normal);
			void loadSprite(const std::string& path, float x = 0, float y = 0, ButtonStatus status = Normal);
			sf::Sprite& getSprite(ButtonStatus status = Normal);
			void createMenu();
			void SetEvent(std::string event);
			void OneShotHint(std::string hint);

			bool isClicked();
			bool isHovering();

			void draw() override;
			void lateUpdate() override;
			void update() override;
		private:
			std::map<std::string, ButtonFunctionality> functions;
			bool _normalSpriteLoaded = false;
			bool _hoveringSpriteLoaded = false;
			bool _clickingSpriteLoaded = false;
			sf::Sprite _normalSprite;
			sf::Sprite _hoverSprite;
			sf::Sprite _clickSprite;

			bool _isHovering;
			bool _isClicked;
			ButtonFunctionality _function;
			ButtonStatus _status = Normal;
			void onHover();
			void onClick();
			void onNotHover();
		};
	}
}
