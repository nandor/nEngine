/**
	@file NPC.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "NPC.hpp"
#include "Console.hpp"
#include "PathFinder.hpp"
using namespace boost::property_tree;

namespace nEngine {
	
	// ------------------------------------------------------------------
	NPC::NPC(const std::string& id, const std::string& base)
		:Object(id, base, "NPC"),
		 mWander(false),
		 mWanderRadius(0),
		 mSelected(false),
		 mCanAttack(true),
		 mDamage(20),
		 mAttackSpeed(3000.0f),
		 mLastAttack(0.0f)
	{
		DataSource* script = getScript();
		mName = script->getValue <std::string> ("name");
		mWander = script->getValue<bool> ("wander");

		if (mWander) {
			mWanderRadius = script->getValue<int> ("wanderRadius");
		}
	}
	
	
	// ------------------------------------------------------------------
	NPC::~NPC()
	{

	}
	
	
	// ------------------------------------------------------------------
	void NPC::update()
	{
		if (mAttacker) {
			float time = Timer::inst().getTime();
			if (time >= mLastAttack + mAttackSpeed) {
				mLastAttack = time;
				mAttacker->damage(mDamage);
			}
		}

		Object::update();
		
		if (mWander && mMoveQueue.size() <= 0 && !mAttacker) {
			Vec2 next = PathFinder(Scene::inst().getMap(), mTile).getRandomNeighbour();
			mMoveQueue.push_back(next);
		}
	}
	
	// ------------------------------------------------------------------
	void NPC::draw()
	{
		int mapSize = Scene::inst().getMap()->getSize();	
		int z = (int)(mapSize - mDrawOn.getX() + mDrawOn.getY()) * mapSize;

		glPushMatrix();
		glTranslatef(mPos.getX(), mPos.getY() + 30, z) ;
		
		Shader::useProgram("npc");
		Shader::setUniformi("uSelected", mSelected);
		Shader::setUniformi("uWidth", mCurrentAnimation.getFrameWidth());
		Shader::setUniformi("uHeight", mCurrentAnimation.getFrameHeight());

		mCurrentAnimation.draw();

		// Draw the health bar
		if (mMaxHealth > mHealth && mDamageable) {
			glDisable(GL_DEPTH_TEST);
			Shader::useProgram("color");
			
			int height = mCurrentAnimation.getFrameHeight();

			glColor3f(0.9f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
				glVertex2i(20, -height -15);
				glVertex2i(100, -height - 15);
				glVertex2i(100, -height - 5);
				glVertex2i(20, -height - 5);
			glEnd();
			
			
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_QUADS);
				glVertex2i(20, -height -15);
				glVertex2i(20 + 80.0 * mHealth / mMaxHealth, -height - 15);
				glVertex2i(20 + 80.0 * mHealth / mMaxHealth, -height - 5);
				glVertex2i(20, -height - 5);
			glEnd();

			glEnable(GL_DEPTH_TEST);
		}

		glPopMatrix();
	}
	
	// ------------------------------------------------------------------
	void NPC::drawMarker()
	{
	}

	// ------------------------------------------------------------------
	void NPC::setSpawn(Vec2& spawn)
	{
		mSpawn = spawn;
		setTile(mSpawn);
	}

	// ------------------------------------------------------------------
	bool NPC::intersects(Vec2& v)
	{
		int height = mCurrentAnimation.getFrameHeight();
		int width = mCurrentAnimation.getFrameWidth();

		return mPos + Vec2(0, - height) < v && v < mPos + Vec2(width, 0);
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(NPC, new)
	{
		NPC* npc = new NPC(std::string(luaL_checkstring(L, 1)), std::string(luaL_checkstring(L, 2)));
		luaInstance(L, NPC, npc);
		return 1;
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(NPC, __setter)
	{
		NPC* npc = *(NPC**)luaGetInstance(L, 1, "NPC");
		std::string field(luaL_checkstring(L, 2));

		if (field == "spawn") {
			npc->setSpawn(**(Vec2**)luaGetInstance(L, 3, "Vec2"));
			return 0;
		}

		if (field == "wander") {
			npc->setWander(lua_toboolean(L, 3));
			return 0;
		}

		if (field == "wanderRadius") {
			npc->setWanderRadius(luaL_checknumber(L, 3));
			return 0;
		}

		return 0;
	}
	

	// ------------------------------------------------------------------
	luaBeginMeta(NPC)
		luaMethod(NPC, __setter)
	luaEndMeta();

	luaBeginMethods(NPC)
		luaMethod(NPC, new)
	luaEndMethods();

	// ------------------------------------------------------------------
	bool luaRegisterNPC(lua_State* L)
	{
		luaInheritClass(L, NPC, Object);
		return true;
	}
};