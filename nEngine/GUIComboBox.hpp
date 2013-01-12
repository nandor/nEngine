/**
	@file GUIComboBox.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUICOMBOBOX_HPP
#define GUICOMBOBOX_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUIComboBox : public GUIElement  {
	public:
		NAPI GUIComboBox(const std::string& id);
		NAPI ~GUIComboBox();
		
		NAPI void addItem(int value, std::string text);

		NAPI unsigned getSelection()
		{
			return mSelectedItem;
		}

	private:
		bool isUnderMouse(Vec2 pos);

		void onDraw();
		void onMouseMove(GUIEvent& evt);
		void onMouseDown(GUIEvent& evt);
		void onMouseUp(GUIEvent& evt);
		void onMouseLeave(GUIEvent& evt);

		unsigned mSelectedItem;
		std::vector <std::pair<int, std::string> > mItems;
		std::string mCaption;

		bool mExpand, mExpandOver, mExpandDown, mOverItem;
		unsigned mHoverItem;
	};
};

#endif /*GUICOMBOBOX_HPP*/