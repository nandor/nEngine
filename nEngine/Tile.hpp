/**
	@file Tile.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef TILE_H
#define TILE_H

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"
#include "Error.hpp"
#include "Lua.hpp"

namespace nEngine {
    class Tile {
    public:
        
		NAPI Tile (Vec2 _pos);
        NAPI ~Tile ();

        NAPI int getID () 
		{
			return mID;
		}

        NAPI void setID (int id)
		{
			mID = id;
		}

        NAPI bool isBlocked () 
		{
			return mBlocked;
		}

        NAPI void setBlocked (bool blocked) 
		{
			mBlocked = blocked;
		}

		NAPI bool hasAction() 
		{
			return mFunc != LUA_NOREF;
		}

        NAPI void setAction(int ref) 
		{
			mFunc = ref;
		}

		NAPI int getAction()
		{
			return mFunc;
		}

		NAPI Vec2 getPosition()
		{
			return mPos;
		}

		NAPI void setExplored(bool explored)
		{
			mExplored = explored;
		}

		NAPI bool isExplored()
		{
			return mExplored;
		}

		NAPI bool isVisible()
		{
			return mVisible;
		}

		NAPI void setVisible(bool visible)
		{
			mVisible = visible;
		}

        NAPI void useAction();

        NAPI static bool luaRegister (lua_State* L);

    private:
        /// Is the tile blocked?
		bool mBlocked;

		/// ID of the tile
        int mID;
		
		/// Reference to the lua function
		int mFunc;
		
		/// Position of the tile
		Vec2 mPos;

		/// Is the tile highlighted ?
		bool mVisible;

		/// Is the tile explored ?
		bool mExplored;
    };
};
#endif /*TILE_H*/
