#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		enum TextAlignment { Left_Justified, Right_Justified, Middle_Justified };

		class TextHUD
		{
		public:
			TextHUD();
			TextHUD(sf::RenderWindow* window);
			~TextHUD() = default;
			TextHUD(const TextHUD& other) = default;
			TextHUD& operator=(const TextHUD& other) = default;

			void setTextInformation(const std::string& text);
			std::string getTextInformation() const;

			sf::Text& getTextObject();
			sf::Font& getFontObject();
			void setFont(const sf::Font& font);
			void setFont(const std::string& fontPath);

			void setWindow(sf::RenderWindow* window);
			void setTextAlignment(TextAlignment alignment);
			TextAlignment getTextAlignment() const;

			void draw() const;
		private:
			//std::unique_ptr<sf::Text> 
			sf::RenderWindow * _window = nullptr;

			std::string _info;

			sf::Font _font;
			sf::Text _text;
			TextAlignment _alignment = Left_Justified;
		};
	}
}
