/**
	@file Light.hpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Lua.hpp"
#include "SceneNode.hpp"

namespace nEngine {
	class Light {
	public:
		/**
			Create a new light source with the given id
			@param id			ID of the light
		*/
		Light(const std::string& id);
		
		/**
			Destroy & clean up the light
		*/
		~Light();
		
		NAPI void glUse(unsigned id);

		/**
			Get the linear attenuation of the source
		*/
		NAPI float getRange()
		{
			return mRange;
		}

		NAPI void setRange(float range)
		{
			mRange = range;
		}

		NAPI std::string getID()
		{
			return mID;
		}

		NAPI Vec2 getTile()
		{
			return mTile;
		}

		NAPI void setTile(Vec2& tile) 
		{
			mTile = tile;
		}

		NAPI void setColor(Color& c)
		{
			mColor = c;
		}

		NAPI Color getColor()
		{
			return mColor;
		}

		NAPI unsigned getGLID()
		{
			return mGLID;
		}

	private:
		
		/// Light attenuation
		float mRange;

		/// ID of the light
		std::string mID;

		/// Position of the light
		Vec2 mTile;

		/// Color
		Color mColor;

		/// OpenGL id of the light
		int mGLID;
	};

	bool luaRegisterLight(lua_State* L);
};

#endif /*LIGHT_HPP*/
