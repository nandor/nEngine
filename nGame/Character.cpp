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
	:Object(id, "pete", "Character")
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
	int mapSize = Scene::inst().getMap()->getSize();
	Tile* t = Scene::inst().getMap()->getTile(mTile);
	
	int z = (int)(mapSize - mDrawOn.getX() + mDrawOn.getY()) * mapSize;

	glPushMatrix();
	glTranslatef(mPos.getX(), mPos.getY() + 30, z) ;

	Shader::useProgram("character");
	Shader::setUniformf("uAmbient", Light::getAmbient());
	Shader::setUniformf("uPosX", mPos.getX());
	Shader::setUniformf("uPosY", mPos.getY());
	Shader::setUniformi("uLight", t->getLight());

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
	c->setTile(Vec2(5, 5));
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