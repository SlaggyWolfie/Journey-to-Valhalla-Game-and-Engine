#pragma once
class LuaScript
{
public:
	LuaScript();
	void LuaScript::registerFunctions();
	void LuaScript::GameLoop();
	void LuaScript::MovePlayer();
	void LuaScript::ShowHint();
	void LuaScript::OpenDoor();
};

