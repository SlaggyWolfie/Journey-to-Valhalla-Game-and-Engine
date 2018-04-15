#include "LuaScript.h"
#include <lua/lua.hpp>
#include <iostream>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/Game.hpp"
#include "../../_vs2015/ColliderManager.h"
#include "../../_vs2015/PlayerBaseComponent.h"
#include "../../_vs2015/LastposStasher.h"
#include "../_vs2015/Time.hpp"
#include "../_vs2015/Button.hpp"
#include "../_vs2015/Sprite.hpp"
#include "../../_vs2015/Sound.hpp"
#include "../_vs2015/Text.hpp"
using namespace Engine;
using namespace Engine::UI;
std::string LuaScript::message = std::string();

LuaScript::LuaScript()
{
	//sf::Text text = sf::Text();

	state_ = luaL_newstate();

	luaL_openlibs(state_);

	if (luaL_dofile(state_, "main.lua") != 0) {
		printf("Read from file-Success\n");
	}
	else {
		printf("Read from file-Failure\n");

	}
	registerFunctions();
}
void LuaScript::start()
{
	std::cout << "initializing Lua" << std::endl;

	lua_getglobal(state_, "Start"); // pushes global update
	int isFunc = lua_isfunction(state_, -1);
	if (isFunc)
	{
		if (lua_pcall(state_, 0, 0, 0) != 0)
		{
			return;
		}
	}
}
glm::vec3 LuaScript::GetPos()
{
	return lastPos;
}
void LuaScript::registerFunctions()
{
	lua_newtable(state_);
	lua_pushcfunction(state_, MovePlayer);
	lua_setfield(state_, -2, "MovePlayer");

	lua_pushcfunction(state_, ShowHint);
	lua_setfield(state_, -2, "ShowHint");

	lua_pushcfunction(state_, PushBackObj);
	lua_setfield(state_, -2, "PushBackObj");

	lua_pushcfunction(state_, NewButton);
	lua_setfield(state_, -2, "NewButton");

	lua_pushcfunction(state_, NewSprite);
	lua_setfield(state_, -2, "NewSprite");

	lua_pushcfunction(state_, NewText);
	lua_setfield(state_, -2, "NewText");

	lua_pushcfunction(state_, SetButtonSpriteNormal);
	lua_setfield(state_, -2, "SetButtonSpriteNormal");

	lua_pushcfunction(state_, SetButtonSpriteHover);
	lua_setfield(state_, -2, "SetButtonSpriteHover");

	lua_pushcfunction(state_, SetButtonSpriteClick);
	lua_setfield(state_, -2, "SetButtonSpriteClick");

	lua_pushcfunction(state_, AddToMenu);
	lua_setfield(state_, -2, "AddToMenu");

	lua_pushcfunction(state_, CollisionBetween);
	lua_setfield(state_, -2, "CollisionBetween");

	lua_pushcfunction(state_, SphereCollisionBetween);
	lua_setfield(state_, -2, "SphereCollisionBetween");

	lua_pushcfunction(state_, GetGameTime);
	lua_setfield(state_, -2, "GetGameTime");

	lua_pushcfunction(state_, SetEvent);
	lua_setfield(state_, -2, "SetEvent");

	lua_pushcfunction(state_, PlaySoundOneShot);
	lua_setfield(state_, -2, "PlaySoundOneShot");

	lua_pushcfunction(state_, SetUIPosition);
	lua_setfield(state_, -2, "SetUIPosition");

	lua_pushcfunction(state_, SetText);
	lua_setfield(state_, -2, "SetText");

	lua_setglobal(state_, "Game");

	std::cout << "registered functions" << std::endl;

}

void LuaScript::update()
{
	lua_getglobal(state_, "Update");
	int isFunc = lua_isfunction(state_, -1);
	if (isFunc) {
		if (lua_pcall(state_, 0, 0, 0) != 0) {
			return;
		}
	}
	//prints message tha is written from lua
}
int LuaScript::MovePlayer(lua_State * state)
{
	return 0;
}

void LuaScript::Initialize()
{
	//std::cout << "initializing Lua" << std::endl;

	lua_getglobal(state_, "Initialize"); // pushes global update
	int isFunc = lua_isfunction(state_, -1);
	if (isFunc) {
		if (lua_pcall(state_, 0, 0, 0) != 0) {
			return;
		}
	}
}

int LuaScript::ShowHint(lua_State * state)
{
	if (lua_gettop(state) == 1 && lua_isstring(state, 1)) {
		//text.setString("start message");
		message = (std::string)lua_tostring(state, 1);
		//message += "\n";
		//if (lua_pcall(state, 0, 0, 0) != 0) {
		//}
		//oneshotHint()
		Engine::UI::Text::hint()->startPosition = sf::Vector2f(600, 1300);
		Engine::UI::Text::hint()->showHint(message, sf::Vector2f(600, 800), 3);
		//text.setString((sf::String)lua_tostring(state, 1));
		//text.setCharacterSize((int)lua_tonumber(state, 4));
		//text.setPosition(lua_tonumber(state, 2), lua_tonumber(state, 3));
		return 0;
	}
	return luaL_error(state, "Object.ShowMessage( string) faulty arguments");
}
//int LuaScript::KeyDown(lua_State * state)
//{
//	if (lua_gettop(state) == 1 && lua_isstring(state, 1)) {
//		//std::cout << "movoing";
//		lua_pushboolean(state, KeyHandler::KeyDown((std::string)lua_tostring(state, 1)));
//		return 1;
//	}
//	return luaL_error(state, "Object.KeyDown( key ), faulty arguments");
//}
void LuaScript::stackDump(lua_State *state, const char * pre) {
	//printf("Lua Stack (%s)> ", pre);
	int i;
	int top = lua_gettop(state);
	for (i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(state, i);
		switch (t) {

		case LUA_TSTRING:  /* strings */
						   //printf("(%d)%s'", i, lua_tostring(state, i));
			break;

		case LUA_TBOOLEAN:  /* booleans */
							//printf("(%d)%s", i, lua_toboolean(state, i) ? "true" : "false");
			break;

		case LUA_TNUMBER:  /* numbers */
						   //printf("(%d)%g", i, lua_tonumber(state, i));
			break;

		default:  /* other values */
				  //printf("(%d)%s", i, lua_typename(state, t));
			break;

		}
		//printf("  ");  /* put a separator */
	}
	//printf("\n");  /* end the listing */
}
int LuaScript::OpenDoor(lua_State * state)
{
	return 0;

}

int LuaScript::OnTriggerEnter(lua_State * state)
{
	return 0;
}

int LuaScript::PushBackObj(lua_State * state)
{
	if (lua_isstring(state, 1) && lua_gettop(state) == 2 && lua_isstring(state, 2))
	{
		//lua_gettop(state)
		//std::string name =lua_tostring(state, 1);
		//ColliderManager* _colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
		//auto comp = _colliderManager->GetColliderByName(name)->getGameObject()->getComponent<PlayerBaseComponent>();
		//if (comp != nullptr) std::cout<<glm::to_string(comp->lp);
		std::string name1 = (std::string)lua_tostring(state, 1);
		std::string name2 = (std::string)lua_tostring(state, 2);
		ColliderManager* _colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
		_colliderManager->GetColliderByName(name1)->PushBackObj(name1, name2);
		//std::cout<<comp->lp;
		//_colliderManager->GetColliderByName(name)->getGameObject()->getTransform()->setPosition(glm::vec3(-300, -599, 300));
		//(_colliderManager->GetColliderByName(name)->getGameObject()->getComponent<PlayerBaseComponent>()->lp);

		return 0;
	}
	return luaL_error(state, " faulty arguments");
}

int LuaScript::CollisionBetween(lua_State * state)
{
	if (lua_isstring(state, 1) && lua_gettop(state) == 2 && lua_isstring(state, 2))
	{
		std::string name1 = (std::string)lua_tostring(state, 1);
		std::string name2 = (std::string)lua_tostring(state, 2);

		ColliderManager* _colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
		bool result = _colliderManager->CheckBoxCollisionBetween(_colliderManager->GetColliderByName(name1), _colliderManager->GetColliderByName(name2));
		lua_pushboolean(state, result);
		//std::cout << result << std::endl;
		//std::cout << _colliderManager->CollisionBetween(_colliderManager->GetColliderByName(name1), _colliderManager->GetColliderByName(name2))<<std::endl;

		//std::cout<<_colliderManager->GetColliderByName(name2);

		return 1;
	}
	return luaL_error(state, " faulty arguments");

}

int LuaScript::SphereCollisionBetween(lua_State * state)
{
	if (lua_isstring(state, 1) && lua_gettop(state) == 2 && lua_isstring(state, 2))
	{
		std::string name1 = (std::string)lua_tostring(state, 1);
		std::string name2 = (std::string)lua_tostring(state, 2);

		ColliderManager* _colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
		bool result = _colliderManager->CollisionBetween(_colliderManager->GetColliderByName(name1), _colliderManager->GetColliderByName(name2));
		lua_pushboolean(state, result);
		//std::cout << result << std::endl;
		//std::cout << _colliderManager->CollisionBetween(_colliderManager->GetColliderByName(name1), _colliderManager->GetColliderByName(name2))<<std::endl;

		//std::cout<<_colliderManager->GetColliderByName(name2);

		return 1;
	}
	return luaL_error(state, " faulty arguments");

}

int LuaScript::GetGameTime(lua_State * state)
{
	lua_pushnumber(state, Engine::Utility::Time::now());
	//std::cout << "getting here" << std::endl;
	return 1;
}

int LuaScript::NewButton(lua_State * state)
{
	if (lua_isstring(state, 1) && lua_isnumber(state, 2) && lua_isnumber(state, 3))
	{
		std::string path = (std::string) lua_tostring(state, 1);
		float x = (float)lua_tonumber(state, 2);
		float y = (float)lua_tonumber(state, 3);

		Engine::UI::Button* btn = new Engine::UI::Button(true);
		btn->loadSprite(path);
		btn->getSprite().setPosition(sf::Vector2f(x, y));
		//btn->setEnabled(false);

		lua_pushlightuserdata(state, btn);
		return 1;
	}
	return luaL_error(state, " faulty arguments");
}

int LuaScript::NewSprite(lua_State* state)
{
	if (lua_isstring(state, 1)
		&& lua_isnumber(state, 2) && lua_isnumber(state, 3)
		&& lua_isnumber(state, 4) && lua_isnumber(state, 5)
		)
	{
		std::string path = (std::string) lua_tostring(state, 1);
		float x = (float)lua_tonumber(state, 2);
		float y = (float)lua_tonumber(state, 3);
		float sizeX = (float)lua_tonumber(state, 4);
		float sizeY = (float)lua_tonumber(state, 5);

		Engine::UI::Sprite* sprite = new Engine::UI::Sprite(true);
		sprite->loadSprite(path);
		sprite->getSprite().setPosition(sf::Vector2f(x, y));

		auto windowSize = ServiceLocator::instance()->getService<Game>()->getWindow()->getSize();
		if (std::abs(sizeX + 1) < 0.01f) sizeX = windowSize.x;
		if (std::abs(sizeY + 1) < 0.01f) sizeY = windowSize.y;
		auto oldSize = sprite->getTexture().getSize();
		//std::cout << sizeX << std::endl;
		//std::cout << sizeY << std::endl;
		sprite->getSprite().setScale(sizeX / oldSize.x, sizeY / oldSize.y);

		//btn->setEnabled(false);

		lua_pushlightuserdata(state, sprite);
		return 1;
	}
	else
		return luaL_error(state, " faulty arguments");
}

int LuaScript::NewText(lua_State* state)
{
	if (lua_isstring(state, 1)
		&& lua_isnumber(state, 2) && lua_isnumber(state, 3)
		&& lua_isnumber(state, 4) && lua_isstring(state, 5)
		&& lua_isstring(state, 6)
		)
	{
		std::string text = (std::string) lua_tostring(state, 1);
		float x = (float)lua_tonumber(state, 2);
		float y = (float)lua_tonumber(state, 3);
		float size = (float)lua_tonumber(state, 4);
		std::string stringColor = (std::string)lua_tostring(state, 5);
		std::string font = (std::string)lua_tostring(state, 6);

		UI::Text* textObject = new UI::Text(true);
		textObject->setTextInformation(text);
		textObject->setFont(font);
		textObject->getTextObject().setCharacterSize(size);
		textObject->getTextObject().setPosition(x, y);

		const sf::Color color = UI::Text::getColor(stringColor);
		textObject->getTextObject().setFillColor(color);

		lua_pushlightuserdata(state, textObject);
		return 1;
	}
	else
		return luaL_error(state, " faulty arguments");
}

int LuaScript::AddToMenu(lua_State * state)
{
	if (lua_isstring(state, 1) && lua_islightuserdata(state, 2))
	{

		int n = lua_gettop(state);
		std::string name = (std::string)lua_tostring(state, 1);

		if (Button::menus.find(name) == Button::menus.end())
		{
			Button::menus[name] = std::vector<ComponentUI*>();
			//std::cout << "Not Found " + name << std::endl;
		}

		std::vector<ComponentUI*>& components = Button::menus[name];
		for (int i = 2; i <= n; i++)
		{
			ComponentUI* component = (ComponentUI*)lua_topointer(state, i);
			components.push_back(component);
		}

		return 0;
	}
	//"MainMenu",btn1,btn2,btn3,btn4,btn5
	return luaL_error(state, " faulty arguments");
}

int LuaScript::SetEvent(lua_State * state)
{
	if (lua_islightuserdata(state, 1) && lua_isstring(state, 2))
	{
		Button* btn = (Button*)lua_topointer(state, 1);
		std::string event = (std::string)lua_tostring(state, 2);
		btn->SetEvent(event);
		return 0;
	}
	return luaL_error(state, " faulty arguments");

}

int LuaScript::SetButtonSpriteNormal(lua_State * state)
{
	if (lua_islightuserdata(state, 1) && lua_isstring(state, 2))
	{
		Button* btn = (Button*)lua_topointer(state, 1);
		std::string path = (std::string)lua_tostring(state, 2);
		btn->loadSprite(path, Normal);

		return 0;
	}
	return luaL_error(state, " faulty arguments");

}

int LuaScript::SetButtonSpriteHover(lua_State * state)
{
	if (lua_islightuserdata(state, 1) && lua_isstring(state, 2))
	{
		Button* btn = (Button*)lua_topointer(state, 1);
		std::string path = (std::string)lua_tostring(state, 2);
		btn->loadSprite(path, Hovering);

		return 0;
	}
	return luaL_error(state, " faulty arguments");

}

int LuaScript::SetButtonSpriteClick(lua_State * state)
{
	if (lua_islightuserdata(state, 1) && lua_isstring(state, 2))
	{
		Button* btn = (Button*)lua_topointer(state, 1);
		std::string path = (std::string)lua_tostring(state, 2);
		btn->loadSprite(path, Clicked);

		return 0;
	}
	return luaL_error(state, " faulty arguments");

}

int LuaScript::SetUIPosition(lua_State* state)
{
	if (lua_islightuserdata(state, 1) && lua_isstring(state, 2) && lua_isnumber(state, 3) && lua_isnumber(state, 4))
	{
		std::string type = (std::string)lua_tostring(state, 2);
		float x = (float)lua_tonumber(state, 3);
		float y = (float)lua_tonumber(state, 4);

		std::transform(type.begin(), type.end(), type.begin(), ::tolower);
		if (type == "button")
		{
			UI::Button* button = (UI::Button*)lua_topointer(state, 1);
			button->getSprite().setPosition(x, y);
		}
		else if (type == "text")
		{
			UI::Text* button = (UI::Text*)lua_topointer(state, 1);
			button->getTextObject().setPosition(x, y);
		}
		else if (type == "sprite")
		{
			UI::Sprite* button = (UI::Sprite*)lua_topointer(state, 1);
			button->getSprite().setPosition(x, y);
		}
		else return luaL_error(state, " faulty arguments");

		return 0;
	}

	return luaL_error(state, " faulty arguments");
}

int LuaScript::SetText(lua_State* state)
{
	if (lua_islightuserdata(state, 1) && lua_isstring(state, 2))
	{
		UI::Text* textObject = (UI::Text*)lua_topointer(state, 1);
		std::string text = (std::string)lua_tostring(state, 2);
		textObject->setTextInformation(text);
		return 0;
	}

	return luaL_error(state, " faulty arguments");
}

int LuaScript::PlaySoundOneShot(lua_State* state)
{
	if (lua_isstring(state, 1))
	{
		//std::cout << "Getting here" << std::endl;
		std::string path = (std::string)lua_tostring(state, 1);
		Audio::Sound::playOneShot(path);
		return 0;
	}
	return luaL_error(state, " faulty arguments");
}




