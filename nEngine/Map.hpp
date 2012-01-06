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

		NAPI bool isVisible(int i, int j);
		NAPI bool isVisible(Vec2& pos);

		NAPI int getMemoryUsage();
		
        NAPI Vec2 getSize ();
        NAPI Vec2 getSpawn ();

        NAPI FieldType* getFieldType(int id);		
		NAPI void callLuaMethod(const std::string& method);

        NAPI static void luaRegister (lua_State* L);

	private:

		void loadMapData (const std::string& fileName);

	private:
		
		int mSize;
		int limitArea;
	
        Vec2 spawn;
        Tile** mData;
        std::map<int, FieldType> fields;
		std::string mNamespace;

	public:

		static const ResourceType sType = RESOURCE_MAP;
	};
};

#endif /*MAP_HPP*/


