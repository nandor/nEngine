/**
	@file GUILabel.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUILABEL_HPP
#define GUILABEL_HPP

#include "types.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUILabel : public GUIElement {
	public:
		NAPI GUILabel(const std::string& id);
		NAPI ~GUILabel();
	
		/**
			Draw event
		*/
		NAPI void onDraw();

		/**
			Set the text of the label
			@param text			New text
		*/
		NAPI void setText(const std::string& text)
		{
			mText = text;
		}

		/**
			Get the text of the label
			@return				Text
		*/
		NAPI std::string getText()
		{
			return mText;
		}
	private:
		std::string mText;
	};
};

#endif /*GUILABEL_HPP*/