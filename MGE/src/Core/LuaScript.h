#pragma once
#include <lua/lua.hpp>
#include <string>
#include "../_vs2015/Component.hpp"
using namespace Engine::Core;

class LuaScript: public Component
{
public:
	LuaScript();
	void LuaScript::registerFunctions();
	static int LuaScript::MovePlayer(lua_State* state);
	void LuaScript::Initialize();
	static int LuaScript::ShowHint(lua_State * state);
	static int LuaScript::OpenDoor(lua_State * state);
	void printMsg();
	void LuaScript::stackDump(lua_State *state, const char * pre);
	void update() override;
	void start() override;
private:
	lua_State *  state_;
	static std::string LuaScript::message;
};

