/**
	@file Object.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Object.hpp"
#include "Map.hpp"
#include "Scene.hpp"
#include "PathFinder.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	Object::Object (const std::string& id, const std::string& obj, const std::string& type)
		 :SceneNode(id, type),
		  mDrawOn(0, 0),
		  mPos(0, 0),
		  mIsMoving(false),
		  mAttacker(NULL),
		  mAttackRange(5.0f)
	{	
		mScriptName = obj;
		DataSource* script = getScript();

		if (script == NULL) {
			throw Error ("Object", getID(), "Invalid object script!");
		}
		
		mHealth = mMaxHealth = script->getValue<int> ("maxHealth");
		mMoveSpeed = script->getValue<int> ("movementSpeed");
		mDamageable = script->getValue<bool> ("damageable");
		
		mAnimRun = false;
		mMoveDir = 0;

		mCurrentAnimation = script->getAnimation("idle");
		mCurrentAnimation.run(-1.0f);
	}
	
	// ------------------------------------------------------------------
	Object::~Object()
	{

	}
	
	// ------------------------------------------------------------------
	void Object::update()
	{
		float time = Timer::inst().getTime();
		float length = mMoveSpeed;
		
		DataSource* script = getScript();
		
		if (mAnimRun) {
			float d = float(time - anim_begin_time) / length;

			if (d < 1.0f) {
				Vec2 u = (anim_target - anim_init) * d;

				mPos = anim_init + u;
			} else {
				mAnimRun = false;
				mPos = anim_target;	
				mDrawOn = mTile;
			}
		}

		if (!mAnimRun) {
			if (!mMoveQueue.empty () && !Scene::inst().getNodeAt(mMoveQueue.front())) {
				anim_begin_time = time;
				anim_init = mPos;

				Vec2 target = mMoveQueue.front ();	
				mMoveQueue.pop_front ();

				anim_target.setX((target.getY() + target.getX()) * TILE_WIDTH / 2);
				anim_target.setY((target.getY() - target.getX()) * TILE_HEIGHT / 2);

				mMoveDir = MapHelper::getDirection(mTile, target);
			
				if (mMoveDir == 0 || mMoveDir == 6) {
					mDrawOn = mTile;
				} else {
					mDrawOn = target;
				}

				std::string dir(boost::lexical_cast<std::string>(mMoveDir));
				std::string newID("move_" + dir);

				if (newID != mCurrentAnimation.getID()) {
					mCurrentAnimation = script->getAnimation("move_" + dir);
				}

				mCurrentAnimation.run(0.0f);

				mTile = target;

				mAnimRun = true;
				mIsMoving = true;
			} else {
				setIdle();
			}
		}
	}

	// ------------------------------------------------------------------
	void Object::setIdle()
	{
		mIsMoving = false;
		mCurrentAnimation.run(-1.0f);

		mMoveQueue.clear();
	}

	// ------------------------------------------------------------------
	void Object::damage(int dmg)
	{
		if (!mDamageable) {
			return;
		}

		mHealth = std::max(0, mHealth - dmg);
	}

	// ------------------------------------------------------------------
	void Object::moveTo (Vec2& target)
	{
		Map* map = Scene::inst().getMap();
		std::vector<Vec2> p = PathFinder(map, mTile, target).getPath();
		
		mMoveQueue.clear ();
		std::copy (p.rbegin(), p.rend(), std::back_inserter(mMoveQueue));
	}
		
	// ------------------------------------------------------------------
	Vec2 Object::getPosition()
	{
		return mPos;
	}

	// ------------------------------------------------------------------
	void Object::setTile(Vec2 tile)
	{
		mDrawOn = mTile = tile;
		mPos.setX((tile.getY() + tile.getX()) * TILE_WIDTH / 2);
		mPos.setY((tile.getY() - tile.getX()) * TILE_HEIGHT / 2);
	}

	
	// ------------------------------------------------------------------
	DataSource* Object::getScript()
	{
		return Resources::inst().require<DataSource> (mScriptName);
	}
		
	// ------------------------------------------------------------------
	luaDeclareMethod(Object, __getter)
	{
		Object* obj = *(Object**)luaGetInstance(L, 1, "Object");
		std::string field(luaL_checkstring(L, 2));

		return 0;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(Object, __setter)
	{
		Object* obj = *(Object**)luaGetInstance(L, 1, "Object");
		std::string field(luaL_checkstring(L, 2));

		if (field == "tile") {
			Vec2* vec = *(Vec2**)luaGetInstance(L, 3, "Vec2");
			obj->setTile(*vec);
			return 0;
		}

		if (field == "position") {
			Vec2* vec = *(Vec2**)luaGetInstance(L, 3, "Vec2");
			obj->setPosition(*vec);
			return 0;
		}
		
		if (field == "health") {
			obj->setHealth(luaL_checkinteger(L, 1));
			return 0;
		}

		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Object)
		luaMethod(Object, __setter)
		luaMethod(Object, __getter)
	luaEndMeta()
	
	// ------------------------------------------------------------------
	luaBeginMethods(Object)

	luaEndMethods()

	// ------------------------------------------------------------------
	bool luaRegisterObject(lua_State* L)
	{
		luaInheritClass(L, Object, SceneNode);
		return true;
	}
};
