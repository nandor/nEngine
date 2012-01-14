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

	glPushMatrix();
	glTranslatef(mPos.getX(), mPos.getY() + 30, (pt.getX() - mDrawOn.getX()) * pt.getY() + mDrawOn.getY()) ;
	
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