/**
	@file GUICheckbox.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUICheckbox.hpp"
#include "GUI.hpp"

namespace nEngine {
	
	// ------------------------------------------------------------------
	GUICheckbox::GUICheckbox(const std::string& id)
		:GUIElement(id),
		 mCaption(""),
		 mChecked(false)
	{
		connect(GUI_EVENT_CLICK, boost::bind(&GUICheckbox::onClick, this, _1));
	}
	
	
	// ------------------------------------------------------------------
	GUICheckbox::~GUICheckbox()
	{

	}
	
	
	// ------------------------------------------------------------------
	void GUICheckbox::onDraw()
	{
		float colorDiff = (mMousePressed ? 0.9 : (mMouseOver ? 1.1 : 1.0));

		if (mFontName != "") {
			Font* ft = Resources::inst().get<Font> (mFontName);
			
			std::string string = mCaption;

			unsigned textWidth = ft->getTextWidth(mCaption);
			if (textWidth >= mComputedSize.getX() - 40) { 
				unsigned len = 0, length = mComputedSize.getX() - 4 * ft->getCharWidth('.') - 40;
				textWidth = 0;
				while (textWidth < length && len < mCaption.length()) {
					textWidth += ft->getCharWidth(mCaption[len++]);
				}
				
				string = mCaption.substr(0, len - 1) + "...";
				textWidth += 3 * ft->getCharWidth('.') - ft->getCharWidth(mCaption[len]);
			}

			glPushMatrix();
			glTranslatef(mComputedSize.getX(), mComputedSize.getY() / 2, 0.0f);
			// draw the checkbox
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_QUADS);
				glVertex2i(-32, -15);
				glVertex2i(-2, -15);
				glVertex2i(-2, 15);
				glVertex2i(-32,15);
			glEnd();

			glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
			glBegin(GL_LINE_LOOP);
				glVertex2i(- 32, -15);
				glVertex2i(- 2, -15);
				glVertex2i(- 2, 15);
				glVertex2i(- 32, 15);
				glVertex2i(- 32, -15);
			glEnd();
			
			if (mChecked) {
				glColor4f(0.0f, 0.7f, 0.0, 1.0f);
				glBegin(GL_QUADS);
					glVertex2i(-24, -7);
					glVertex2i(-10, -7);
					glVertex2i(-10, 7);
					glVertex2i(-24, 7);
				glEnd();
			}

			
			glPopMatrix();
			if (mEnabled) {
				float colorDiff = (mMousePressed ? 0.9 : (mMouseOver ? 1.1 : 1.0));

				(mFontColor * colorDiff).glUse();		
			} else {
				glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
			}
			glPrint(ft, 0, (mSize.getY() + ft->getHeight()) / 2, string);

		}
	}
	
	
	// ------------------------------------------------------------------
	void GUICheckbox::onClick(GUIEvent& evt)
	{
		mChecked = !mChecked;
	}
};
