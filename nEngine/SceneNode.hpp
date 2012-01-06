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
		SceneNode(const std::string& id, const std::string& type);
		virtual ~SceneNode();
		
		/**
			Draw the object
		*/
        virtual void draw() = 0;

		/**
			Update the object
		*/
		virtual void update() = 0;
		
		/**
			Set the id of the object
			@param id				ID of the object
		*/
		void setID(const std::string& id)
		{
			mID = id;
		}

		/**
			Get the id of the object
			@return				ID of the object
		*/
		std::string getID()
		{
			return mID;
		}

		/**
			Set the tile
			@param tile			New tile
		*/
		virtual void setTile(Vec2 tile)
		{
			mTile = tile;
		}
		
		/**
			Get the tile
			@return				Tile
		*/
		Vec2 getTile()
		{
			return mTile;
		}

		/**
			Get the type
			@return type
		*/
		std::string getType()
		{
			return mType;
		}

		void setHightlighted(bool highlight)
		{
			mHighlighted =  highlight;
		}

		bool isHighlighted()
		{
			return mHighlighted;
		}

		void setHighlightRange(int range)
		{
			mHighlightRange = range;
		}

		int getHighlightRange()
		{
			return mHighlightRange;
		}

		virtual Vec2 getPosition();
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
	};

	bool luaRegisterSceneNode(lua_State* L);
};

#endif /*SCENENODE_HPP*/