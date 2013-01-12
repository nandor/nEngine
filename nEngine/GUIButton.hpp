/**
	@file GUIButton.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include "GUIElement.hpp"

namespace nEngine {
	class GUIButton : public GUIElement  {
	public:
		NAPI GUIButton(const std::string& id);
		NAPI ~GUIButton();

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
			Set the caption of the button
			@param str		New caption
		*/
		NAPI void setCaption(const std::string& str)
		{
			mCaption = str;
		}
	private:
		std::string mCaption;
	};

	bool luaRegisterGUIButton(lua_State* L);

};

#endif /*GUIBUTTON_HPP*/