/**
	@file Tile.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef TILE_H
#define TILE_H

#include "types.hpp"
#include "Vec2.hpp"
#include "Lua.hpp"

namespace nEngine {
    class Tile {
    public:
        
		/**
			Create a new tile
			@param pos			Position of the tile
		*/
		NAPI Tile (Vec2 pos);
        
		/**
			Destroy the tile
		*/
		NAPI ~Tile ();

		/**
			Get the id of the field type present on the tile
		*/
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

		NAPI float getLight()
		{
			return mLights;
		}

		/**
			Set a bitmask for the used lights
			@param light			i-th bit is 1 if i-th light
		*/
		NAPI void setLight(float light)
		{
			mLights = light;
		}

		NAPI void addLight(unsigned id);

		NAPI int getHeight()
		{
			return mHeight;
		}

		NAPI void setHeight(int height)
		{
			mHeight = height;
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

		/// IDs of lights that affect the tile
		int mLights;

		/// Height of the tile
		int mHeight;
    };
};

#endif /*TILE_H*/
