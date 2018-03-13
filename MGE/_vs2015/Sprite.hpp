#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "ComponentUI.hpp"

namespace Engine
{
	namespace UI
	{
		class Sprite : public ComponentUI
		{
		public:
			Sprite(bool rendering = true);
			Sprite(sf::RenderWindow* window, bool rendering = true);
			virtual ~Sprite() = default;
			Sprite(const Sprite& other) = default;
			Sprite& operator=(const Sprite& other) = default;

			sf::Sprite& getSpriteObject();
			sf::Texture& getTexture();
			void loadSprite(const sf::Texture& texture);
			void loadSprite(const std::string& texturePath);
			void changeTexture(const sf::Texture& texture);
			void changeTexture(const std::string& texturePath);

			void draw() override;
		private:
			//std::unique_ptr<sf::Sprite> 
			sf::Sprite _sprite;
			sf::Texture _texture;
		};
	}
}
