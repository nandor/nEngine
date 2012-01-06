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
		mPos = Vec2(0, 0);
		mSize = Vec2(getScreenSize());
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
		
		Shader::useProgram("gui");

		GUIElement::draw();

		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}
	
	
	// ------------------------------------------------------------------
	bool GUI::handleEvent(GUIEvent& evt)
	{
		bool captured = false;
		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			captured |= it->second->handleEvent(evt);
		}

		return captured;
	}
};