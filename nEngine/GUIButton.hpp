/**
	@file GUIButton.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"
#include "GUISizer.hpp"

namespace nEngine {
	class GUIButton : public GUIElement  {
	public:
		GUIButton(const std::string& id);
		~GUIButton();

		void onDraw();

		/**
			Get the caption of the button
			@return caption
		*/
		std::string getCaption()
		{
			return mCaption;
		}


		/**
			Set the caption of the button
			@param str		New caption
		*/
		void setCaption(const std::string& str)
		{
			mCaption = str;
		}
	private:
		std::string mCaption;
	};
};

#endif /*GUIBUTTON_HPP*/