/**
	@file GUIButton.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUICHECKBOX_HPP
#define GUICHECKBOX_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUICheckbox : public GUIElement  {
	public:
		NAPI GUICheckbox(const std::string& id);
		NAPI ~GUICheckbox();

		NAPI void onDraw();

		/**
			Get the caption of the button
			@return caption
		*/
		NAPI std::string getCaption()
		{
			return mCaption;
		}
		
		/**
			Check the box
			@param checked Check state
		*/
		NAPI void setChecked(bool value)
		{
			mChecked = value;
		}

		NAPI bool getChecked()
		{
			return mChecked;
		}

		/**
			Set the caption of the button
			@param str		New caption
		*/
		NAPI void setCaption(const std::string& str)
		{
			mCaption = str;
		}
	private:

		void onClick(GUIEvent& evt);

		std::string mCaption;
		bool mChecked;
	};
};

#endif /*GUICHECKBOX_HPP*/