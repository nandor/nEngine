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
#include "Tile.hpp"
#include "Light.hpp"
#include "Resource.hpp"
#include "MapHelper.hpp"

#define TILE_WIDTH 120
#define TILE_HEIGHT 60

namespace nEngine {
    class FieldType {
	public:
		NAPI FieldType();
		NAPI ~FieldType();

		/// Id of the field
		int mID;

        /// Name of the tile
        std::string mName;
        
		/// Filename of the tile
        std::string mImage;

		/// True if the player can't step on the tile
		bool mBlocked;

		/// Height of the field
		int mHeight;
    };

    /**
        @brief Load & manage map data
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
        NAPI bool hasTile(Vec2& tile);

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
			Get the ammount of memory used by the map
			@return			Memory in bytes
		*/
		NAPI int getMemoryUsage();
		
		/**
			Get the size of the map
			@return			Size of the map (width & height)
		*/
        NAPI int getSize ()
		{
			return mSize;
		}

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
			Change the id of the tile
			@param pos			Position of the tile
			@param tile			Id of the tile
		*/
		NAPI void setTileID(Vec2 pos, int tile);

		/**
			Write the map data
			@param fileName		Name of the output file. If fileName is "", the old one is rewritten
		*/
		NAPI void writeMapData(const std::string& fileName = "");

		/**
			Build the map data
			@return				Pointer to an array containing map data
		*/
		NAPI std::pair<uint8*, unsigned> buildMapData();

		/**
			Get the name of the namespace
			@return				String containing namespace name
		*/
		NAPI std::string getNamespace()
		{
			return mNamespace;
		}

		/**
			Check if the tile is blocked
			@param pos		Position o f the tile
		*/
		NAPI bool isBlocked(const Vec2& v);

		/**
			Check if the tile is blocked
			@param i		X coordinate
			@param j		Y coordinate
		*/
		NAPI bool isBlocked(int i, int j);
		
		/**
			Enable or disable lighting
			@param light		True if lighting is enabled
		*/
		NAPI void setLighting(bool light)
		{
			mLighting = light;
		}

		/**
			Build the lightmap based on the selected lights
			@param lights		Map containing lights
		*/
		NAPI void buildLightMap(std::map<std::string, Light*>& lights);

	private:

		/**
			Load map data
			@param fileName				source file
		*/
		void loadMapData (const std::string& fileName);

		/**
			Get the height of a tile
			@param x					X coordinate
			@param y					Y coordinate
		*/
		int getHeight(int i, int j);

		/**
			Return the minimum height on the line from p0 to p1
			@param x0
			@param y0
			@param x1
			@param y1
			
			@return minimum height
		*/
		int getMaxHeight(int x0, int y0, int x1, int y1);
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
		
		/// Name of the file containing map data
		std::string mDataName;

		/// Is lighting enables?
		bool mLighting;

	public:
		
		static const ResourceType sType = RESOURCE_MAP;
	};

    NAPI bool luaRegisterMap (lua_State* L);
};

#endif /*MAP_HPP*/


