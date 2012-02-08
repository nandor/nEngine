/**
	@file GUI.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUI_HPP
#define GUI_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUI : public Singleton<GUI>, public GUIElement {
	public:
		NAPI GUI();
		NAPI ~GUI();

		/**
			Draw the entire GUI
		*/
		NAPI void draw();

		/**
			Handle an event
			@param evt		Event to be processed
			@return			True if an element caught it, false if it did not affect anything
		*/
		NAPI bool handleEvent(GUIEvent& evt);

		/**
			Remove an element
			@param id		ID of the element
		*/
		void remove(const std::string& id);

		/**
			Find an element
			@param id		ID of the element
		*/
		NAPI GUIElement* get(const std::string& id);

		/**
			Mouse position
			@return			Position of the mouse
		*/
		NAPI Vec2 getMousePos()
		{
			return mMousePos;
		}

	private:
		/// Add an element to the global array
		bool addToGlobal(GUIElement* elem);

		/// Map containing pointer to all elements
		std::map<std::string, GUIElement*> mGlobal;

		/// Just a friend
		friend class GUIElement;

		/// Mouse position
		Vec2 mMousePos;
	};

	NAPI bool luaRegisterGUI(lua_State* L);
};
#endif /*GUI_HPP*/