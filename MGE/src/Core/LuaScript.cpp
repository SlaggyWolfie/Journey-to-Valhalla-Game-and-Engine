#include "LuaScript.h"
#include <list>
#include <lua/lua.hpp>
#include <iostream>
#include <string>

std::string LuaScript::message = nullptr;

LuaScript::LuaScript()
{
	//sf::Text text = sf::Text();
	LuaScript::message = "";

	state_ = luaL_newstate();

	luaL_openlibs(state_);

	registerFunctions();
	luaL_dofile(state_, "main.lua");
	Initialize();
}

void LuaScript::Initialize()
{
	std::cout << "initializing" << std::endl;
	lua_getglobal(state_, "Initialize"); // pushes global update
	int isFunc = lua_isfunction(state_, -1);
	//if (isFunc) {
	//	if (lua_pcall(state, 0, 0, 0) != 0) {
	//	}
	//}

	GameLoop();
}
void LuaScript::registerFunctions()
{
	lua_newtable(state_);
	lua_pushcfunction(state_, MovePlayer);
	lua_setfield(state_, -2, "MovePlayer");

	lua_pushcfunction(state_, ShowHint);
	lua_setfield(state_, -2, "ShowHint");


}

void LuaScript::GameLoop()
{

}
int LuaScript::MovePlayer(lua_State * state)
{
	return 0;
}
int LuaScript::ShowHint(lua_State * state)
{


	if (lua_gettop(state) == 4 && lua_isstring(state, 1) && lua_isnumber(state, 2) && lua_isnumber(state, 3) && lua_isnumber(state, 4)/* && lua_isnumber(state, 5)*/) {

		//text.setString("start message");
		message = (std::string)lua_tostring(state, 1);
		message += "\n";
		//text.setString((sf::String)lua_tostring(state, 1));
		//text.setCharacterSize((int)lua_tonumber(state, 4));
		//text.setPosition(lua_tonumber(state, 2), lua_tonumber(state, 3));
		std::cout << message;
		return 0;
	}
	return 0;

}
int LuaScript::OpenDoor(lua_State * state)
{
	return 0;

}
