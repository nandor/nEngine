/**
	@file GUIPanel.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIPanel.hpp"

namespace nEngine {
	
	
	// ------------------------------------------------------------------
	GUIPanel::GUIPanel(const std::string& id)
		:GUIElement(id)
	{

	}
	
	
	// ------------------------------------------------------------------
	GUIPanel::~GUIPanel()
	{

	}
	
	
	// ------------------------------------------------------------------
	void GUIPanel::onDraw()
	{		
		mBackgroundColor.glUse();

		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(mSize.getX(), 0);
			glVertex2i(mSize.getX(), mSize.getY());
			glVertex2i(0, mSize.getY());
		glEnd();	
	}
};