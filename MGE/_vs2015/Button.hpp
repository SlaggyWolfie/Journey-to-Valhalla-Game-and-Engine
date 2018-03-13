#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "ComponentUI.hpp"

namespace Engine
{
	namespace UI
	{
		enum ButtonStatus { Normal, Hovering, Clicked };
		enum ButtonFunctionality { LoadLevel, Exit, Options, Credits };

		class Button : public ComponentUI
		{
		public:
			Button(bool rendering = true);
			Button( sf::RenderWindow* window, bool rendering = true);
			virtual ~Button() = default;
			Button(const Button& other) = default;
			Button& operator=(const Button& other) = default;

			void loadSprite(const sf::Sprite& texture, ButtonStatus status = Normal);
			void loadSprite(const std::string& path, ButtonStatus status = Normal);
			sf::Sprite& getSprite(ButtonStatus status = Normal);
			void createMenu();
			void OneShotHint(std::string hint);

			void draw() override;
			void lateUpdate() override;
		private:
			bool _normalSpriteLoaded = false;
			bool _hoveringSpriteLoaded = false;
			bool _clickingSpriteLoaded = false;
			sf::Sprite _normalSprite;
			sf::Sprite _hoverSprite;
			sf::Sprite _clickSprite;

			ButtonStatus _status = Normal;
			void onHover();
			void onClick();
			void onNotHover();
		};
	}
}
