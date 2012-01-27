/**
	@file GUIEvent.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIEvent.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	GUIEvent::GUIEvent()
	{

	}
	
	// ------------------------------------------------------------------
	GUIEvent::GUIEvent(GUIEventType type)
	{
		mType = type;
	}

	// ------------------------------------------------------------------
	GUIEvent::GUIEvent(GUIEventType type, int x, int y)
	{
		mType = type;
		mMouseX = x;
		mMouseY = y;
	}
	
	
	// ------------------------------------------------------------------
	GUIEvent::GUIEvent(GUIEventType type, Vec2 mouse)
	{
		mType = type;
		mMouseX = mouse.getX();
		mMouseY = mouse.getY();
	}

	
	// ------------------------------------------------------------------
	GUIEvent::~GUIEvent ()
	{

	}
};