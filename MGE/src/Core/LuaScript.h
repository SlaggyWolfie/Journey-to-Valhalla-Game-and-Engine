#pragma once
#include <lua/lua.hpp>
#include <string>
class LuaScript
{
public:
	LuaScript();
	void LuaScript::registerFunctions();
	void LuaScript::GameLoop();
	static int LuaScript::MovePlayer(lua_State* state);
	void LuaScript::Initialize();
	static int LuaScript::ShowHint(lua_State * state);
	static int LuaScript::OpenDoor(lua_State * state);
private:
	lua_State *  state_;
	static std::string LuaScript::message;
};

