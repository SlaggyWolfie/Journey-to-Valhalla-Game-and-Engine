#include "LuaScript.h"
#include <lua/lua.hpp>
#include <iostream>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Core/Game.hpp"
#include "../../_vs2015/ColliderManager.h"
#include "../../_vs2015/PlayerBaseComponent.h"
#include "../../_vs2015/LastposStasher.h"

std::string LuaScript::message = std::string();

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
void LuaScript::start()
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

	lua_pushcfunction(state_, CollisionBetween);
	lua_setfield(state_, -2, "CollisionBetween");


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
//	parent = getGameObject()->getComponent<collider>();
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
		std::cout << message << std::endl;
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

int LuaScript::OnTriggerEnter(lua_State * state)
{
	return 0;
}

int LuaScript::PushBackObj(lua_State * state)
{
	if (lua_isstring(state, 1) && lua_gettop(state) == 2 && lua_isstring(state, 2))
	{
		std::string name1 = (std::string)lua_tostring(state, 1);
		std::string name2 = (std::string)lua_tostring(state, 2);
		ColliderManager* _colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
		_colliderManager->GetColliderByName(name1)->PushBackObj(name1,name2);
		//std::cout<<comp->lp;

		//_colliderManager->GetColliderByName(name)->getGameObject()->getTransform()->setPosition(glm::vec3(-300, -599, 300));
		//(_colliderManager->GetColliderByName(name)->getGameObject()->getComponent<PlayerBaseComponent>()->lp);
		
	return 0;
	}
	return luaL_error(state, " faulty arguments");
}

int LuaScript::CollisionBetween(lua_State * state)
{
	if (lua_isstring(state, 1)&& lua_gettop(state) == 2 && lua_isstring(state, 2))
	{
		std::string name1= (std::string)lua_tostring(state, 1);
		std::string name2 = (std::string)lua_tostring(state, 2);

		ColliderManager* _colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
		bool result = _colliderManager->CollisionBetween(_colliderManager->GetColliderByName(name1), _colliderManager->GetColliderByName(name2));
		lua_pushboolean(state,result);
		//std::cout << result << std::endl;
		//std::cout << _colliderManager->CollisionBetween(_colliderManager->GetColliderByName(name1), _colliderManager->GetColliderByName(name2))<<std::endl;

		//std::cout<<_colliderManager->GetColliderByName(name2);

		return 1;
	}
	return luaL_error(state, " faulty arguments");

}
