/**
	@file GUI.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUI.hpp"

namespace nEngine {
	template<> GUI* GUI::Singleton<GUI>::mInstance = NULL;
		
	// ------------------------------------------------------------------
	GUI::GUI()
		:GUIElement("")
	{
		mComputedPos = mPos = Vec2(0, 0);
		mComputedSize = mSize = Vec2(getScreenSize());
		mParent = NULL;
	}		
	
	// ------------------------------------------------------------------
	GUI::~GUI()
	{

	}	
	
	// ------------------------------------------------------------------
	void GUI::draw()
	{
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_DEPTH_TEST);
		
		Shader::useProgram("color");

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(mMousePos.getX(), mMousePos.getY(), 0.0f);
		glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(50, 0);
		glVertex2i(50, 50);
		glVertex2i(0, 50);
		glEnd();
		glPopMatrix();
		
		Shader::useProgram("gui");
		GUIElement::draw();

		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}
	
	
	// ------------------------------------------------------------------
	bool GUI::handleEvent(GUIEvent& evt)
	{
		if (evt.getType() == GUI_EVENT_MOUSEMOVE) {
			mMousePos = evt.getMousePos();
		}

		bool captured = false;
		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			captured |= it->second->handleEvent(evt);
		}

		return captured;
	}
	
	
	// ------------------------------------------------------------------
	bool GUI::addToGlobal(GUIElement* elem)
	{
		std::map<std::string, GUIElement*>::const_iterator it = mGlobal.find(elem->getID());

		if (it != mGlobal.end()) {
			return false;
		}

		mGlobal.insert(std::make_pair(elem->getID(), elem));
		return true;
	}

	// ------------------------------------------------------------------
	GUIElement* GUI::get(const std::string& id)
	{
		std::map<std::string, GUIElement*>::const_iterator it = mGlobal.find(id);

		if (it == mGlobal.end()) {
			return NULL;
		}

		return it->second;
	}
};