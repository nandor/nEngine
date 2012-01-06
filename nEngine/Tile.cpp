/**
	@file Timer.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Tile.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	Tile::Tile (Vec2 pos)
		:mBlocked(false),
		 mID(0),
		 mFunc(LUA_NOREF),
		 mPos(pos),
		 mExplored(false),
		 mVisible(false)
	{

	}
	
	
	// ------------------------------------------------------------------
	Tile::~Tile ()
	{

	}
		
	// ------------------------------------------------------------------
	void Tile::useAction()
	{
		if (mFunc == LUA_NOREF) {
			throw Error("Tile", "Invalid reference!");
		}

		lua_State* L = luaGlobalState();
		
		lua_rawgeti(L, LUA_REGISTRYINDEX, mFunc);
		luaInstance(L, Tile, this);
		lua_pcall(L, 1, 0, 0);	
	}
		

	luaNewMethod(Tile, __setter)
	{
		Tile* tile = *(Tile**)luaGetInstance(L, 1, "Tile");

		std::string field(luaL_checkstring(L, 2));

		if (field == "id") {
			tile->setID(luaL_checkinteger(L, 3));
			lua_pushvalue(L, 3);
			return 1;
		}

		if (field == "blocked") {
			if (lua_isboolean(L, 3)) {
				tile->setBlocked(lua_toboolean(L, 3));
				lua_pushvalue(L, 3);
				return 1;
			}
			return 0;
		}

		if (field == "action") {
			lua_pushvalue(L, 3);
			tile->setAction(luaL_ref(L, LUA_REGISTRYINDEX));
			return 1;
		}

		return 0;
	}

	luaNewMethod(Tile, __getter)
	{
		Tile* tile = *(Tile**)luaGetInstance(L, 1, "Tile");

		std::string field(luaL_checkstring(L, 2));

		if (field == "id") {
			lua_pushinteger(L, tile->getID());
			return 1;
		}

		if (field == "blocked") {
			lua_pushboolean(L, tile->isBlocked());
			return 1;
		}

		if (field == "action") {
			lua_rawgeti(L, LUA_REGISTRYINDEX, tile->getAction());
			return 1;
		}

		if (field == "x") {
			lua_pushinteger(L, tile->getPosition().getX());
			return 1;
		}

		if (field == "y") {
			lua_pushinteger(L, tile->getPosition().getY());
			return 1;
		}

		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Tile)
		luaMethod(Tile, __setter)
		luaMethod(Tile, __getter)
	luaEndMeta()
	

	// ------------------------------------------------------------------
	luaBeginMethods(Tile)
	luaEndMethods()
	

	// ------------------------------------------------------------------
	bool Tile::luaRegister (lua_State* L)
	{
		luaClass(L, Tile);
		return true;
	}
};