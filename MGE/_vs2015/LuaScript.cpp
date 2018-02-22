#include "Core\LuaScript.h"
#include <list>
#include <lua/lua.hpp>
#include <iostream>
#include <string>


std::string LuaScript::message;

LuaScript::LuaScript()
{
	//sf::Text text = sf::Text();

	state_ = luaL_newstate();

	luaL_openlibs(state_);

	if (luaL_dofile(state_, "main.lua") !=0 ) {
		printf("Read from file-Success\n");
	}
	else {
		printf("Read from file-Success\n");

	}
	registerFunctions();

	Initialize();
}
void LuaScript::Initialize()
{
	std::cout << "initializing Lua" << std::endl;

	lua_getglobal(state_, "Initialize"); // pushes global update
	int isFunc = lua_isfunction(state_, -1);
	if (isFunc) {
		if (lua_pcall(state_, 0, 0, 0) != 0) {
			return;
		}
	}
}
void LuaScript::registerFunctions()
{
	lua_newtable(state_);
	lua_pushcfunction(state_, MovePlayer);
	lua_setfield(state_, -2, "MovePlayer");

	lua_pushcfunction(state_, ShowHint);
	lua_setfield(state_, -2, "ShowHint");

	lua_setglobal(state_, "Game");

	std::cout << "registered functions" << std::endl;

}

void LuaScript::GameLoop()
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
int LuaScript::ShowHint(lua_State * state)
{

	if (lua_gettop(state) == 4 && lua_isstring(state, 1) && lua_isnumber(state, 2) &&
		lua_isnumber(state, 3) && lua_isnumber(state, 4))
	{
		//text.setString("start message");
		message = (std::string)lua_tostring(state, 1);
		message += "\n";
		//if (lua_pcall(state, 0, 0, 0) != 0) {
		//}
		
		//text.setString((sf::String)lua_tostring(state, 1));
		//text.setCharacterSize((int)lua_tonumber(state, 4));
		//text.setPosition(lua_tonumber(state, 2), lua_tonumber(state, 3));
		return 0;
	}
	return luaL_error(state, "Object.ShowMessage( string,x,y, size), faulty arguments");
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
void stackDump(lua_State *state, const char * pre) {
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
