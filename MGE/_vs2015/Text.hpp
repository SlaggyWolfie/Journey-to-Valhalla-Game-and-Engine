#pragma once
#include <SFML/Graphics.hpp>
#include "ComponentUI.hpp"

namespace Engine
{
	namespace UI
	{
		enum TextAlignment { Left_Justified, Right_Justified, Middle_Justified };

		struct Hint
		{
			Text* text = nullptr;
			bool draw = false;
			bool move = false;
			bool durationSet = false;
			bool showing = false;
			sf::Vector2f targetPosition;
			sf::Vector2f startPosition;
			float duration = 0;

			//static void showHint(const std::string& hintText, float startAfterTime = 0, float duration = 6);
			void showHint(const std::string& hintText, sf::Vector2f target, float duration = 6);
			void startCountdown();
			void shouldDraw(bool show);
			void setPosition(float x, float y) const;
			void setPosition(sf::Vector2f position) const;

			Hint(bool rendering = false);
			void update();
		};

		class Text : public ComponentUI
		{
		public:
			static Hint* hint();
			//static Text* hint();
			//static bool drawHint;
			//static void showHint(const std::string& hintText, float startAfterTime = 0, float duration = 6);
			//static void showHint2(const std::string& hintText, float duration = 6);
			//static void shouldDrawHint(bool show);
			//static void hintSetPosition(float x, float y);
			//static sf::Vector2f hintTargetPosition;

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
			//static Text* _hint;
			static sf::Vector2f moveTowards(sf::Vector2f current, const sf::Vector2f target, const float delta);
		private:
			//std::unique_ptr<sf::Text> 
			//sf::RenderWindow * _window = nullptr;

			static Hint* _hint;

			std::string _info;

			sf::Font _font;
			sf::Text _text;

			TextAlignment _alignment = Left_Justified;
		};
	}
}
