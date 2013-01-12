/**
    @file       Map.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Map.hpp"
#include "File.hpp"
#include "Scene.hpp"
#include "Image.hpp"
#include "Shader.hpp"
#include "PathFinder.hpp"
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
		 mTileSize(120.0f, 60.0f),
		 mLighting(true)
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
				int id = boost::lexical_cast<int> (v.first);
				FieldType& f = mFields[id];
				f.mID = id;
				f.mImage = v.second.get<std::string> ("image");
				f.mName = v.second.get<std::string> ("name");
				f.mBlocked = v.second.get<bool> ("blocked");
				f.mHeight = v.second.get<int> ("height");
			}

			mNamespace = data.get<std::string> ("namespace");
			loadMapData(data.get<std::string>("mapData"));
		
		} catch (ptree_error err) {
			throw Error ("Map error ", getID(), err.what()); 	
		}
	}

	// ------------------------------------------------------------------
	void Map::loadMapData(const std::string& fileName)
	{
		mDataName = fileName;
		File* file =  Resources::inst().require<File>(fileName);
		
		mData = new Tile* [mSize * mSize];

		for (int i = 0; i < mSize * mSize; ++i) {
			Tile* t = mData[i] = new Tile(Vec2(i / mSize, i % mSize));
			
			fdata* d = ((fdata*)file->getData()) + i;

			FieldType& f = getFieldType(d->id);

			t->setID(d->id);
			t->setHeight(f.mHeight);
			t->setBlocked(d->blocked);
		}
	}
	

	// ------------------------------------------------------------------
	void Map::writeMapData(const std::string& fileName)
	{
		std::string name = File::processName((fileName.size() == 0) ? mDataName : fileName);
		std::pair<uint8*, unsigned> data = buildMapData();

		File* f = Resources::inst().require<File> (mDataName);
		f->copyData(data.first, data.second);

		FILE* fout = fopen(name.c_str(), "wb");
		
		if (!fout) {
			throw Error("Map", getID(), "Cannot open file for writing: '" + name + "'");
		}

		if (fwrite(data.first, sizeof(uint8), data.second, fout) != data.second) {
			fclose(fout);
			throw Error("Map", getID(), "Cannot write data in: '" + name + "'");
		}

		fclose(fout);
		delete[] data.first;
	}

	// ------------------------------------------------------------------
	std::pair<uint8*, unsigned> Map::buildMapData()
	{
		unsigned length = mSize * mSize * sizeof(fdata);
		uint8* data = new uint8[length];
		memset(data, 0, sizeof(data));

		for (int  i = 0; i < mSize * mSize; ++i) {
			fdata* d = ((fdata*)data) + i;
			d->id = mData[i]->getID();
			d->blocked = mData[i]->isBlocked();
		}
		return std::make_pair(data, length);
	}

	// ------------------------------------------------------------------
	void Map::draw()
	{
		Shader::useProgram("tile");
		Shader::setUniformf("ambient", Light::getAmbient());

		Vec2 off = Scene::inst().getCameraOffset(), screenSize = getScreenSize(), tileSize(TILE_WIDTH, TILE_HEIGHT);
	
		int ybeg = std::max(0, std::min((int)(off.getX() / 2 + off.getY()) / (int)tileSize.getY() - 2, mSize - 1));
		int yend = std::max(0, std::min((int)((off.getX() + screenSize.getX()) / 2 + off.getY() + screenSize.getY()) / (int)tileSize.getY() + 2, mSize - 1));
		
		int xbeg = std::max(0, std::min((int)(off.getX() / 2 - off.getY() - screenSize.getY()) / (int)tileSize.getY() - 2, mSize - 1));
		int xend = std::max(0, std::min((int)((off.getX() + screenSize.getX()) / 2 - off.getY()) / (int)tileSize.getY() + 2, mSize - 1));
		
		for (int x = xend; x >= xbeg; --x) {
			for (int y = ybeg; y <= yend; ++y) {
				int trans_x = ((x + y) * TILE_WIDTH) >> 1;
				int trans_y = ((y - x - 1) * TILE_HEIGHT ) >> 1;
				int trans_z = (mSize - x + y) * mSize;

				Tile* t = mData[x * mSize + y];				
				FieldType& f = getFieldType(t->getID());
				Image* img =  Resources::inst().require<Image>(f.mImage);

				int isExplored = 2;
				int width = img->getWidth(), height = img->getHeight() + 1;

				Shader::setUniformi("lights", t->getLight());
				Shader::setUniformf("tileX", trans_x);				
				Shader::setUniformf("tileY", trans_y);

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
	void Map::buildLightMap(std::map<std::string, Light*>& lights)
	{
		int h = getMaxHeight(8, 10, 15, 10);
		typedef std::map<std::string, Light*>::iterator iter;
		
		for (int i = 0; i < mSize * mSize; ++i) {
			mData[i]->setLight(0);
		}

		for (iter it = lights.begin(); it != lights.end(); ++it) {
			Light* l = it->second;

			Vec2 pos = l->getTile();
			int ix = pos.getX(), iy = pos.getY();
			int height = mData[ix * mSize + iy]->getHeight();

			int A = std::ceilf(l->getRange()) + 3;
			
			for (int x = std::max(0, ix - A); x < std::min(mSize, ix + A); ++x) {
				for (int y = std::max(0, iy - A); y < std::min(mSize, iy + A); ++y) {
					if ((double)(ix - x) * (ix - x) + (iy - y) * (iy - y) > A * A) {
						continue;
					}
					
					Tile* t = mData[x * mSize + y];

					if (getMaxHeight(ix, iy, x, y) <= height) {
						t->addLight(l->getGLID());
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------------
	int Map::getHeight(int x, int y)
	{
		return mData[x * mSize + y]->getHeight();
	}

	
	// ------------------------------------------------------------------
	int Map::getMaxHeight(int x0, int y0, int x1, int y1)
	{
		int maxHeight = std::numeric_limits<int>::min();
		int dx = std::abs(x1 - x0), dy = std::abs(y1 - y0);

		int sx = (x0 < x1) ? 1 : -1;
		int sy = (y0 < y1) ? 1 : -1;

		int error = dx - dy;

		while (true) {
			maxHeight = std::max(maxHeight, getHeight(x0, y0));

			if (x0 == x1 && y0 == y1) {
				break;
			}

			int err2 = 2 * error;
			
			if (err2 > -dy) {
				error -= dy;
				x0 += sx;
			}
			if (err2 < dx) {
				error += dx;
				y0 += sy;	
			}
		}

		return maxHeight;
	}

	// ------------------------------------------------------------------
	bool Map::isBlocked(const Vec2& tile) 
	{
		Tile* t = mData[(int)tile.getX() * mSize + (int)tile.getY()];
		FieldType& f = getFieldType(t->getID());
		return t->isBlocked() || f.mBlocked;
	}
	
	// ------------------------------------------------------------------
	bool Map::isBlocked(int x, int y) 
	{
		Tile* t = mData[x * mSize + y];
		FieldType& f = getFieldType(t->getID());
		return t->isBlocked() || f.mBlocked;
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
		lua_pcall(L, 1, 0, 0);
	}

	// ------------------------------------------------------------------
	void Map::setTileID(Vec2 pos, int id)
	{
		Tile* tile = mData[(int)pos.getX() * mSize + (int)pos.getY()];

		tile->setID(id);
	}


	// ------------------------------------------------------------------
	int Map::getMemoryUsage()
	{
		return sizeof(*this) + sizeof(Tile) * mSize * mSize + sizeof(mFields) + sizeof(FieldType) * mFields.size();
	}


	// ------------------------------------------------------------------
	luaDeclareMethod(Map, getSize)
	{
		Map* v = *(Map**)luaGetInstance(L, 1, "Map");
		
		lua_pushnumber(L, v->getSize());
		return 1;
	}

	
	// ------------------------------------------------------------------
	luaDeclareMethod(Map, getTile)
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