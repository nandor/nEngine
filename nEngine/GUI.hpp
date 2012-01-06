/**
	@file GUI.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUI_HPP
#define GUI_HPP

#include "Singleton.hpp"
#include "Shader.hpp"
#include "Util.hpp"
#include "Font.hpp"

// GUI elements
#include "GUIElement.hpp"
#include "GUIButton.hpp"
#include "GUISizer.hpp"
#include "GUIPanel.hpp"
#include "GUILabel.hpp"


namespace nEngine {
	class GUI : public Singleton<GUI>, public GUIElement {
	public:
		GUI();
		~GUI();

		/**
			Draw the entire GUI
		*/
		void draw();

		/**
			Handle an event
			@param evt		Event to be processed
			@return			True if an element caught it, false if it did not affect anything
		*/
		bool handleEvent(GUIEvent& evt);
	private:

	};
};
#endif /*GUI_HPP*/