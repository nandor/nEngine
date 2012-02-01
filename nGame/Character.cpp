/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEngine/Scene.hpp"

#include "Character.hpp"
using namespace nEngine;

#define CHR_ANIM_TIME 500

// ------------------------------------------------------------------
Character::Character(const std::string& id)
	:Object(id, "pete")
{	 
	this->setHighlightRange(4);
	this->setHightlighted(true);
}

// ------------------------------------------------------------------
Character::~Character()
{

}
	
// ------------------------------------------------------------------
void Character::draw ()
{
	Vec2 pt = Scene::inst().getMap()->getSize();
	int z = (pt.getX() - mDrawOn.getX() + mDrawOn.getY()) * pt.getX();

	glPushMatrix();
	glTranslatef(mPos.getX(), mPos.getY() + 30, z) ;
	
	mCurrentAnimation.draw();

	glPopMatrix();
}
	
// ------------------------------------------------------------------
void Character::drawMarker()
{

}

// ------------------------------------------------------------------
void Character::update()
{
	Object::update();
}

// ------------------------------------------------------------------
luaNewMethod(Character, init)
{
	Character* c = new Character("character");
	c->setTile(Vec2(2, 2));
	Scene::inst().addNode(c);
	return 0;
}

// ------------------------------------------------------------------
luaBeginMeta(Character)
luaEndMeta()

luaBeginMethods(Character)
	luaMethod(Character, init)
luaEndMethods()

// ------------------------------------------------------------------
bool luaRegisterCharacter(lua_State* L)
{
	luaInheritClass(L, Character, SceneNode);
	return true;
}