/**
	@file GUIEvent.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUIEVENT_HPP
#define GUIEVENT_HPP

#include "types.hpp"
#include "Vec2.hpp"
#include "Color.hpp"

namespace nEngine {

	/// Event types
	enum GUIEventType {
		GUI_EVENT_MOUSEDOWN,
		GUI_EVENT_MOUSEUP,
		GUI_EVENT_MOUSEMOVE,
		GUI_EVENT_MOUSEENTER,
		GUI_EVENT_MOUSELEAVE,

		GUI_EVENT_MOUSE,

		GUI_EVENT_CLICK,
		
		GUI_NUM_EVENTS
	};

	class GUIEvent {
	public:
		NAPI GUIEvent();
		NAPI GUIEvent(GUIEventType, int mouseX, int mouseY);
		NAPI GUIEvent(GUIEventType, Vec2 mouse);

		NAPI ~GUIEvent();
		
		/**
			Get the type of the element
			@return			Type of the event
		*/
		NAPI GUIEventType getType()
		{
			return mType;
		}

		/**
			Return the position of the mouse
			@return			Vec2 containing mouse position
		*/
		NAPI Vec2 getMousePos()
		{
			return Vec2(mMouseX, mMouseY);
		}

		/**
			Set the position of the mouse
			@param pos		Mouse position
		*/
		NAPI void setMousePos (Vec2 pos)
		{
			mMouseX = pos.getX();
			mMouseY = pos.getY();
		}

	private:
		int mMouseX, mMouseY;
		GUIEventType mType;
	};
};

#endif /*GUIEVENT_HPP*/
