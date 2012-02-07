/**
	@file SceneNode.hpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"
#include "Lua.hpp"

namespace nEngine {
	class SceneNode {
	public:
		NAPI SceneNode(const std::string& id, const std::string& type);
		NAPI virtual ~SceneNode();
		
		/**
			Draw the object
		*/
        NAPI virtual void draw() = 0;

		/**
			Draw a marker
		*/
		NAPI virtual void drawMarker() = 0;

		/**
			Update the object
		*/
		NAPI virtual void update() = 0;
		
		/**
			Check if the object intersects a certain point
		*/
		NAPI virtual bool intersects(Vec2& v) = 0;

		/**
			Set the id of the object
			@param id				ID of the object
		*/
		NAPI void setID(const std::string& id)
		{
			mID = id;
		}

		/**
			Get the id of the object
			@return				ID of the object
		*/
		NAPI std::string getID()
		{
			return mID;
		}

		/**
			Set the tile
			@param tile			New tile
		*/
		NAPI virtual void setTile(Vec2 tile)
		{
			mTile = tile;
		}
		
		/**
			Get the tile
			@return				Tile
		*/
		NAPI Vec2 getTile()
		{
			return mTile;
		}

		/**
			Get the type
			@return type
		*/
		NAPI std::string getType()
		{
			return mType;
		}

		NAPI void setHightlighted(bool highlight)
		{
			mHighlighted =  highlight;
		}

		NAPI bool isHighlighted()
		{
			return mHighlighted;
		}

		NAPI void setHighlightRange(int range)
		{
			mHighlightRange = range;
		}

		NAPI int getHighlightRange()
		{
			return mHighlightRange;
		}

		NAPI virtual Vec2 getPosition();

		void setMarker(bool v)
		{
			mShowMarker = v;
		}

	protected:		
		/// ID of the object
		std::string mID;

		/// Set the type
		std::string mType;
		
		/// The tile of the node
		Vec2 mTile;

		/// Is the object highlighted on the map?
		bool mHighlighted;

		/// Highlight range
		int mHighlightRange;

		/// Show a marker for the element
		bool mShowMarker;
	};

	bool luaRegisterSceneNode(lua_State* L);
};

#endif /*SCENENODE_HPP*/