/**
	@file Map.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef MAP_HPP
#define MAP_HPP

#include "types.hpp"
#include "Lua.hpp"
#include "File.hpp"
#include "Tile.hpp"
#include "Image.hpp"
#include "Shader.hpp"
#include "Resource.hpp"
#include "MapHelper.hpp"

#define TILE_WIDTH 120
#define TILE_HEIGHT 60

namespace nEngine {
    class FieldType {
	public:
		NAPI FieldType();
		NAPI ~FieldType();

		/** Id of the field */
		int mID;

        /** Name of the tile */
        std::string mName;
        
		/** Filename of the tile */
        std::string mImage;
    };

    /**
        @brief Singleton class that holds and manages map data
    */
    class Map : public Resource {
    public:
		/**
			Create a new map
			@param id			ID of the map
		*/
		NAPI Map(const std::string& id);

		/**
			Delete the map
		*/
		NAPI ~Map();
		
		/**
			Load data from the property tree
			@param data			Ptree containing data
		*/
		NAPI void loadData(boost::property_tree::ptree& data);

        /**
            Draws the map
        */
        NAPI void draw();

        /**
            Check if a tile is on the map
            @param tile		Coordinate of the tile
            @return         True/False
        */
        NAPI bool hasTile (Vec2& tile);

		/**
			Check if a tile is on the map
			@param x
			@param y
			@return			Tile
		*/
		NAPI bool hasTile(int x, int y);

		/**
			Get the size of a tile
			@return			Size of a tile
		*/
		NAPI Vec2 getTileSize();
		
		/**
			Get a tile
			@param tile		Coordinate
			@return			Pointer to the tile
		*/
        NAPI Tile* getTile (Vec2& tile);

		/**
			Get a tile
			@param x
			@param y
			@return		
		*/
		NAPI Tile* getTile(int x, int y);


		/**
			Shadow all the tiles
		*/
		NAPI void shadow();

		/**
			Highlight an area 
		*/
		NAPI void highlight(Vec2 pos, int range);

		/**
			Check if a tile is visible
			@param i		X coordinate
			@param j		Y coordinate
			@return			True if fairies are walking on the tile
		*/
		NAPI bool isVisible(int i, int j);
		
		/**
			Check if a tile is visible
			@param pos		Position o f the tile
			@return			True if fairies are walking on the tile
		*/
		NAPI bool isVisible(Vec2& pos);

		/**
			Get the ammount of memory used by the map
			@return			Memory in bytes
		*/
		NAPI int getMemoryUsage();
		
		/**
			Get the size of the map
			@return			Vec2 containing size (Vec2(mSize, mSize))
		*/
        NAPI Vec2 getSize ();

		/**
			Get a reference to a certain field type
			@param id		ID of the field
			@return			The required reference
		*/
        NAPI FieldType& getFieldType(int id);		

		/**
			Return the map containing the filed types
			@return				THE map
		*/
		NAPI std::map<int, FieldType>& getFieldTypes()
		{
			return mFields;
		}

		/**
			Call a lua method from the map's namespace
			@param method		Name of the method
		*/
		NAPI void callLuaMethod(const std::string& method);

		/**
			Enable / disable map shadows (aoe explore map like)
			@param shadow		True / False
		*/
		NAPI void setShadows(bool shadow)
		{
			mShadow = shadow;
		}

	private:

		/**
			Load map data
			@param fileName				source file
		*/
		void loadMapData (const std::string& fileName);

	private:
		
		/// Size of the map (must be a power of two)
		int mSize;
	
		/// Size of a tile (120 x 60 by default)
		Vec2 mTileSize;

		/// Map data
        Tile** mData;

		/// Map containing field types
        std::map<int, FieldType> mFields;
		
		/// Lua namespace name
		std::string mNamespace;

		/// Is the map shadowed?
		bool mShadow;

	public:
		
		static const ResourceType sType = RESOURCE_MAP;
	};

    NAPI bool luaRegisterMap (lua_State* L);
};

#endif /*MAP_HPP*/


