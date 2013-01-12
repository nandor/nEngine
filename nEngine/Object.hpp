/**
	@file Object.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "types.hpp"
#include "Lua.hpp"
#include "Util.hpp"
#include "Timer.hpp"
#include "Animation.hpp"
#include "File.hpp"
#include "DataSource.hpp"
#include "SceneNode.hpp"

namespace nEngine {
    class Object : public SceneNode {
    public:
		/**
			Create a new object
			@param id			ID of the object
			@param script		Object Script
		*/
        NAPI Object(const std::string& id, const std::string& script, const std::string& type);
        
		/**
			Destroy the object
		*/
		NAPI virtual ~Object();

		/**
			Update the object
		*/
		NAPI virtual void update();

		/**
			Move the object to a certain position
			@param v			Target
		*/
		NAPI virtual void moveTo(Vec2& v);

		/**
			Set the position of the object
			@param pos			New position
		*/
		NAPI void setPosition(Vec2 pos)
		{
			mPos = pos;
		}

		/**
			Check if the object is moving
			@return				True or false
		*/
		NAPI bool isMoving()
		{
			return mIsMoving;
		}
		
		/**
			Change the tile of the object
			@param tile			New tile
		*/
		NAPI void setTile(Vec2 tile);

		/**
			Change into an idle state
		*/
        NAPI void setIdle();

		/**
			Set the health of the object
			@param health			New HP
		*/
		NAPI void setHealth(int health)
		{
			mHealth = health;
		}

		/**
			Set a pointer to an NPC who is attacking this one
			@param obj				The attacker
		*/
		NAPI void setAttacker(Object* obj)
		{
			mAttacker = obj;
		}
		
		NAPI bool isDead()
		{
			return mHealth <= 0;
		}

		NAPI void damage(int dmg);

		NAPI DataSource* getScript();

        NAPI virtual Vec2 getPosition();

		float getAttackRange()
		{
			return mAttackRange;
		}

    protected:
		/// Position of the object
		Vec2 mPos;

		/// JSON config
		boost::property_tree::ptree mCfg;

		/// Size of the npc
		Vec2 mSize;

		/// The tile on which the object is drawn
		Vec2 mDrawOn;

		/// Can the object be damaged?
		bool mDamageable;

		/// Attack range
		float mAttackRange;

		/// Maximum health
		int mMaxHealth;

		/// Current health
		int mHealth;

		/// Is moving?
		bool mIsMoving;

		/// Movement speed
		float mMoveSpeed;

		/// Current animation
		Animation mCurrentAnimation;

		/// Movement queue
		std::list<Vec2> mMoveQueue;

		/// Beginning of the last animation
		float anim_begin_time;

		/// Movement direction
		int mMoveDir;

		/// Is an animation running?
		bool mAnimRun;

		/// Script name
		std::string mScriptName;

		/// Who is attacked this entity?
		Object* mAttacker;

		Vec2 anim_init, anim_target;
    };
	
	/**
		Register the object
	*/
	bool luaRegisterObject(lua_State* L);
};
#endif /*OBJECT_HPP*/
