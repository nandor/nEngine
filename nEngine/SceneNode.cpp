/**
	@file SceneNode.cpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "SceneNode.hpp"
#include "Map.hpp"

namespace nEngine {
	
	// ------------------------------------------------------------------
	SceneNode::SceneNode(const std::string& id, const std::string& type)
		:mID(id),
		 mTile(0, 0),
		 mType(type),
		 mHighlighted(false),
		 mHighlightRange(0)
	{

	}
	
	
	// ------------------------------------------------------------------
	SceneNode::~SceneNode()
	{

	}

	// ------------------------------------------------------------------
	Vec2 SceneNode::getPosition()
	{
		return Vec2(
			(mTile.getX() + mTile.getY()) * TILE_WIDTH / 2,
			(mTile.getY() - mTile.getX() - 1) * TILE_HEIGHT / 2
		);
	}

	// ------------------------------------------------------------------
	luaNewMethod(SceneNode, __getter)
	{
		return 0;
	}

	// ------------------------------------------------------------------
	luaNewMethod(SceneNode, __setter)
	{
		SceneNode* obj = *(SceneNode**)luaGetInstance(L, 1, "SceneNode");
		std::string field(luaL_checkstring(L, 2));

		if (field == "tile") {
			Vec2* tile = *(Vec2**)luaGetInstance(L, 3, "Vec2");
			obj->setTile(*tile);
			return 0;
		}

		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(SceneNode)
		luaMethod(SceneNode, __getter)
		luaMethod(SceneNode, __setter)
	luaEndMeta()

	// ------------------------------------------------------------------
	luaBeginMethods(SceneNode)
	luaEndMethods()

	// ------------------------------------------------------------------
	bool luaRegisterSceneNode(lua_State* L)
	{
		luaClass(L, SceneNode);
		return true;
	}
};