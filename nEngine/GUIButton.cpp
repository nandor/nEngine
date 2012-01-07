/**
	@file GUIButton.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIButton.hpp"
#include "GUI.hpp"

namespace nEngine {
	
	
	// ------------------------------------------------------------------
	GUIButton::GUIButton(const std::string& id)
		:GUIElement(id),
		 mCaption("")
	{

	}
	
	
	// ------------------------------------------------------------------
	GUIButton::~GUIButton()
	{

	}
	
	
	// ------------------------------------------------------------------
	void GUIButton::onDraw()
	{
		float colorDiff = (mMousePressed ? 0.9 : (mMouseOver ? 1.1 : 1.0));

		(mBackgroundColor * colorDiff).glUse();		

		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(mSize.getX(), 0);
			glVertex2i(mSize.getX(), mSize.getY());
			glVertex2i(0, mSize.getY());
		glEnd();

		if (mFontName != "") {
			Font* ft = Resources::inst().get<Font> (mFontName);
			
			std::string string = mCaption;

			unsigned textWidth = ft->getTextWidth(mCaption);
			if (textWidth >= mSize.getX()) { 
				unsigned len = 0, length = mComputedSize.getX() - 4 * ft->getCharWidth('.');
				textWidth = 0;
				while (textWidth < length && len < mCaption.length()) {
					textWidth += ft->getCharWidth(mCaption[len++]);
				}
				
				string = mCaption.substr(0, len - 1) + "...";
				textWidth += 3 * ft->getCharWidth('.') - ft->getCharWidth(mCaption[len]);
			}
			
			if (mEnabled) {
				mFontColor.glUse();
			} else {
				glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
			}
			glPrint(ft, (mSize.getX() - textWidth) / 2, (mSize.getY() + ft->getHeight()) / 2, string);
		}
	}
};