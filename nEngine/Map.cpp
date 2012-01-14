/**
    @file       Map.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Map.hpp"
#include "PathFinder.hpp"
#include "Scene.hpp"
using namespace boost::property_tree;

namespace nEngine {

	// ------------------------------------------------------------------
	FieldType::FieldType()
		:mID(0),
		 mImage(""),
		 mName("Void")
	{

	}	

	// ------------------------------------------------------------------
	FieldType::~FieldType()
	{

	}

	
	// ------------------------------------------------------------------
	Map::Map(const std::string& id)
		:Resource(id, RESOURCE_MAP),
		 mShadow(true)
	{

	}
	

	// ------------------------------------------------------------------
	Map::~Map ()
	{
		mFields.clear();
		for (int i = 0; i < mSize * mSize; ++i) {
			delete mData[i];
		}
		delete[] mData;
	}

	#pragma pack(push, 1)

	struct fdata {
		unsigned char id;
		unsigned char blocked;
		unsigned char pad[2];
	};

	#pragma pack(pop)
	
	// ------------------------------------------------------------------
	void Map::loadData(boost::property_tree::ptree& data) {
		try {

			mSize = data.get<int>("size");
		
			if (mSize & (mSize - 1)) {
				throw Error ("Map", getID(), "Size of map is not a power of two!");
			}

			BOOST_FOREACH (ptree::value_type& v, data.get_child("fields")) {
				FieldType& f = mFields[v.second.get<int> ("id")];
				f.mID = v.second.get<int> ("id");
				f.mImage = v.second.get<std::string> ("image");
				f.mName =v.second.get<std::string> ("name");
			}

			mNamespace = data.get<std::string> ("namespace");
			BOOST_FOREACH (ptree::value_type& v, data.get_child("script")) {
				luaReadFile(v.second.get_value<std::string>());
			}

			loadMapData(data.get<std::string>("mapData"));
		} catch (ptree_error err) {
			throw Error ("Map error ", getID(), err.what()); 	
		}
	}

	// ------------------------------------------------------------------
	void Map::loadMapData(const std::string& fileName)
	{
		File* file =  Resources::inst().require<File>(fileName);
		
		mData = new Tile* [mSize * mSize];

		for (int i = 0; i < mSize * mSize; ++i) {
			Tile* t = mData[i] = new Tile(Vec2(i / mSize, i % mSize));

			fdata* d = ((fdata*)file->getData()) + i;
			t->setID(d->id);
			t->setBlocked(d->blocked);
		}
	}
	
	// ------------------------------------------------------------------
	void Map::draw()
	{
		Shader::useProgram("tile");

		Vec2 off = Scene::inst().getCameraOffset(), screenSize = getScreenSize(), tileSize(TILE_WIDTH, TILE_HEIGHT);
	
		int ybeg = max(0, min((off.getX() / 2 + off.getY()) / TILE_HEIGHT, mSize - 1));
		int yend = max(0, min(((off.getX() + screenSize.getX()) / 2 + off.getY() + screenSize.getY()) / tileSize.getY(), mSize - 1));
		
		int xbeg = max(0, min((off.getX() / 2 - off.getY() - screenSize.getY()) / tileSize.getY(), mSize - 1));
		int xend = max(0, min(((off.getX() + screenSize.getX()) / 2 - off.getY()) / tileSize.getY(), mSize - 1));
		
		glColor3f (1.0f, 1.0f, 1.0f);

		for (int x = xend; x >= xbeg; --x) {
			for (int y = ybeg; y <= yend; ++y) {
				int trans_x = ((x + y) * TILE_WIDTH) >> 1;
				int trans_y = ((y - x - 1) * TILE_HEIGHT ) >> 1;
				int trans_z = (mSize - x) * mSize + y;

				Tile* t = mData[x * mSize + y];				
				FieldType& f = getFieldType(t->getID());
				Image* img =  Resources::inst().require<Image>(f.mImage);

				int isExplored = mShadow ? (t->isVisible() ? 2 : (t->isExplored() ? 1 : 0)) : 2;
				int width = img->getWidth(), height = img->getHeight();

				Shader::setUniformi("isExplored", 1, &isExplored);
				Shader::setUniformi("tileWidth", 1, &width); 
				Shader::setUniformi("tileHeight", 1, &height); 
				
				img->bind();
				glBegin (GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex3i (trans_x, trans_y + 60 - height, trans_z);
					glTexCoord2f(1.0f, 0.0f); glVertex3i (trans_x + width, trans_y + 60 - height, trans_z);
					glTexCoord2f(1.0f, 1.0f); glVertex3i (trans_x + width, trans_y + 60, trans_z);
					glTexCoord2f(0.0f, 1.0f); glVertex3i (trans_x, trans_y + 60, trans_z);
				glEnd ();
			}
		}
	}
	
	// ------------------------------------------------------------------
	void Map::shadow()
	{
		if (mShadow) {
			for (int i = 0; i < mSize * mSize; ++i) {
				mData[i]->setVisible(false);
			}
		}
	}
	
	// ------------------------------------------------------------------
	void Map::highlight(Vec2 pos, int range)
	{
		if (mShadow) {
			for (int i = pos.getX() - range; i <= pos.getX() + range; ++i) {
				for (int j = pos.getY() - range; j <= pos.getY() + range; ++j) {
					int dx = abs(i - pos.getX());
					int dy = abs(j - pos.getY());

					if (hasTile(i, j) && dx * dx + dy * dy <= range * range) {
						Tile* t = mData[i * mSize + j];

						t->setExplored(true);
						t->setVisible(true);
					}
				}
			}
		}
	}

	// ------------------------------------------------------------------
	bool Map::isVisible (Vec2& tile)
	{
		Tile* t = mData[(int)tile.getX() * mSize + (int)tile.getY()];
		return t->isVisible();
	}
	
	// ------------------------------------------------------------------
	bool Map::isVisible (int i, int j)
	{
		Tile* t = mData[i * mSize + j];
		return t->isVisible();
	}

	// ------------------------------------------------------------------
	bool Map::hasTile (Vec2& tile)
	{
		return 0 <= tile.getX() && tile.getY() < mSize && 0 <= tile.getY() && tile.getY() < mSize;
	}
		
	// ------------------------------------------------------------------
	bool Map::hasTile (int x, int y)
	{
		return 0 <= x && x < mSize && 0 <= y && y < mSize;
	}
	
	// ------------------------------------------------------------------
	Vec2 Map::getSize ()
	{
		return Vec2(mSize, mSize);
	}
		
	// ------------------------------------------------------------------
	Tile* Map::getTile (Vec2& v)
	{
		return mData[(int)(v.getX() * mSize + v.getY())];
	}

	// ------------------------------------------------------------------
	Tile* Map::getTile (int x, int y)
	{
		return mData[x * mSize + y];
	}
	
	// ------------------------------------------------------------------
	FieldType& Map::getFieldType(int id)
	{
		return mFields[id];
	}
		
	// ------------------------------------------------------------------
	Vec2 Map::getTileSize()
	{
		return Vec2(TILE_WIDTH, TILE_HEIGHT);
	}

	// ------------------------------------------------------------------
	void Map::callLuaMethod(const std::string& name)
	{
		lua_State* L = luaGlobalState();

		lua_pushcfunction(L, luaErrorCallback);
		lua_getglobal(L, mNamespace.c_str());
		
		if (!lua_istable(L, -1)) {
			return;
		}

		lua_pushstring(L, name.c_str());
		lua_gettable(L, -2);

		if (!lua_isfunction(L, -1)) {
			return;
		}

		luaInstance(L, Map, this);
		lua_pcall(L, 1, 0, -4);
	}


	// ------------------------------------------------------------------
	int Map::getMemoryUsage()
	{
		return sizeof(*this) + sizeof(Tile) * mSize * mSize + sizeof(mFields) + sizeof(FieldType) * mFields.size();
	}


	// ------------------------------------------------------------------
	luaNewMethod(Map, getSize)
	{
		Map* v = *(Map**)luaGetInstance(L, 1, "Map");
		
		luaInstance(L, Vec2, new Vec2(v->getSize()));
		return 1;
	}

	
	// ------------------------------------------------------------------
	luaNewMethod(Map, getTile)
	{
		Map* v = *(Map**)luaGetInstance(L, 1, "Map");

		int x = luaL_checknumber(L, 2);
		int y = luaL_checknumber(L, 3);

		luaInstance(L, Tile, v->getTile(x, y));
		return 1;
	}
	

	// ------------------------------------------------------------------
	luaBeginMeta(Map)
		luaMethod(Map, getSize)
		luaMethod(Map, getTile)
	luaEndMeta()
	

	// ------------------------------------------------------------------
	luaBeginMethods(Map)
	luaEndMethods()
	
	// ------------------------------------------------------------------
	bool luaRegisterMap (lua_State* L)
	{
		luaClass(L, Map);
		return true;
	}
};