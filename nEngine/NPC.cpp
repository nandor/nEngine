/**
	@file NPC.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "NPC.hpp"
#include "PathFinder.hpp"
using namespace boost::property_tree;

namespace nEngine {
	
	// ------------------------------------------------------------------
	NPC::NPC(const std::string& id, const std::string& base)
		:Object(id, base),
		 mWander(false),
		 mWanderRadius(0)
	{
		ObjectScript* script = getScript();
		mName = script->getValue <std::string> ("name");
		mWanderRadius = script->getValue<int> ("wanderRadius");
		mWander = script->getValue<bool> ("wander");
	}
	
	
	// ------------------------------------------------------------------
	NPC::~NPC()
	{

	}
	
	
	// ------------------------------------------------------------------
	void NPC::update()
	{
		Object::update();

		if (mWander && mMoveQueue.size() <= 0) {
			Vec2 next = PathFinder(Scene::inst().getMap(), mTile).getRandomNeighbour();
			mMoveQueue.push_back(next);
		}
	}
	
	// ------------------------------------------------------------------
	void NPC::draw()
	{
		Vec2 pt = Scene::inst().getMap()->getSize();

		glPushMatrix();
		glTranslatef(mPos.getX(), mPos.getY() + 30, (pt.getX() - mDrawOn.getX()) * pt.getY() + mDrawOn.getY()) ;
		
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
	
	luaNewMethod(NPC, new)
	{
		NPC* npc = new NPC(std::string(luaL_checkstring(L, 1)), std::string(luaL_checkstring(L, 2)));
		luaInstance(L, NPC, npc);
		return 1;
	}

	luaNewMethod(NPC, setSpawn)
	{
		
		return 0;
	}

	luaBeginMeta(NPC)
		luaMethod(NPC, setSpawn)
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