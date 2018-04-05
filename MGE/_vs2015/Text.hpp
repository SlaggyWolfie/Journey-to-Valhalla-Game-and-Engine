#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "ComponentUI.hpp"

namespace Engine
{
	namespace UI
	{
		enum TextAlignment { Left_Justified, Right_Justified, Middle_Justified };

		class Text : public ComponentUI
		{
		public:
			static Text* hint();
			static bool drawHint;
			static void showHint(const std::string& hintText, float startAfterTime = 0, float duration = 6);
			Text(bool rendering = true);
			Text(sf::RenderWindow* window, bool rendering = true);
			virtual ~Text() = default;
			Text(const Text& other) = default;
			Text& operator=(const Text& other) = default;

			void setTextInformation(const std::string& text);
			std::string getTextInformation() const;

			sf::Text& getTextObject();
			sf::Font& getFontObject();
			void setFont(const sf::Font& font);
			void setFont(const std::string& fontPath);
			void setTextAlignment(TextAlignment alignment);
			TextAlignment getTextAlignment() const;

			void draw() override;
			static Text* _hint;
		private:
			//std::unique_ptr<sf::Text> 
			//sf::RenderWindow * _window = nullptr;

			std::string _info;

			sf::Font _font;
			sf::Text _text;
			sf::Text t;

			TextAlignment _alignment = Left_Justified;
		};
	}
}
