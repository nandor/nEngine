/**
	@file GUIComboBox.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIComboBox.hpp"
#include "GUI.hpp"
#include "Console.hpp"

namespace nEngine {
	
	
	// ------------------------------------------------------------------
	GUIComboBox::GUIComboBox(const std::string& id)
		:GUIElement(id),
		 mCaption(""),
		 mExpandOver(false),
		 mExpand(false),
		 mHoverItem(0)
	{
		mSelectedItem = 0;
		this->connect(GUI_EVENT_MOUSEMOVE, boost::bind(&GUIComboBox::onMouseMove, this, _1));
		this->connect(GUI_EVENT_MOUSEDOWN, boost::bind(&GUIComboBox::onMouseDown, this, _1));
		this->connect(GUI_EVENT_MOUSEUP, boost::bind(&GUIComboBox::onMouseUp, this, _1));
		this->connect(GUI_EVENT_MOUSELEAVE, boost::bind(&GUIComboBox::onMouseLeave, this, _1));
	}
	
	
	// ------------------------------------------------------------------
	GUIComboBox::~GUIComboBox()
	{

	}
	
	
	// ------------------------------------------------------------------
	void GUIComboBox::onDraw()
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(mSize.getX(), 0);
			glVertex2i(mSize.getX(), mSize.getY());
			glVertex2i(0, mSize.getY());
		glEnd();
		
		glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(mSize.getX(), 0);
			glVertex2i(mSize.getX(), mSize.getY());
			glVertex2i(0, mSize.getY());
			glVertex2i(0, 0);
		glEnd();

		Font* ft = Resources::inst().get<Font> (mFontName);
		if (mItems.size() > mSelectedItem) {
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glPrint(ft, 2, (mSize.getY() + ft->getHeight()) / 2, mSize.getX() - 30, mItems[mSelectedItem].second);
		}

		Color buttonColor(0.6f, 0.6f, 0.6f, 1.0f);
		float ColorMult = (mExpandOver ? (mExpandDown ? 0.7 : 1.3) : 1.0); 

		(buttonColor * ColorMult).glUse();
		
		glBegin(GL_TRIANGLES);
			glVertex2i(mSize.getX() - 30, mSize.getY() / 2 - 4);
			glVertex2i(mSize.getX() - 6, mSize.getY() / 2 - 4);
			glVertex2i(mSize.getX() - 18, mSize.getY() / 2 + 8);
		glEnd();

		if (mExpand) {
			unsigned itemHeight = (ft->getHeight() + 3);
			unsigned mHeight = mItems.size() *  itemHeight + 5;

			glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
			glBegin(GL_QUADS);
				glVertex2i(0, mSize.getY());
				glVertex2i(mSize.getX(), mSize.getY());
				glVertex2i(mSize.getX(), mSize.getY() + mHeight);
				glVertex2i(0, mSize.getY() + mHeight);
			glEnd();
					
			if (mOverItem) {
				glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
				glBegin(GL_QUADS);
					glVertex2i(0, mSize.getY() + itemHeight * mHoverItem + 3);
					glVertex2i(mSize.getX(), mSize.getY() + itemHeight * mHoverItem + 3);
					glVertex2i(mSize.getX(), mSize.getY() + itemHeight * (mHoverItem + 1) + 3);
					glVertex2i(0, mSize.getY() + itemHeight * (mHoverItem + 1) + 3);
				glEnd();
			}

			glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
			glBegin(GL_LINE_LOOP);
				glVertex2i(0, mSize.getY() + mHeight);
				glVertex2i(0, mSize.getY());
				glVertex2i(mSize.getX(), mSize.getY());
				glVertex2i(mSize.getX(), mSize.getY() + mHeight);
				glVertex2i(0, mSize.getY() + mHeight);
			glEnd();

			mFontColor.glUse();
			for (unsigned i = 0; i < mItems.size(); ++i) {
				glPrint(ft, 2, mSize.getY() + itemHeight * (i + 1), mSize.getX(), mItems[i].second);
			}
		}
	}

	// ------------------------------------------------------------------
	void GUIComboBox::addItem(int value, std::string text)
	{
		mItems.push_back(std::make_pair(value, text));
	}

	// ------------------------------------------------------------------
	void GUIComboBox::onMouseMove(GUIEvent& evt)
	{
		Vec2 mousePos = evt.getMousePos() - mPos;
		mousePos.setX(mousePos.getX() - mSize.getX() + 30);
		mousePos.setY(mousePos.getY() - mSize.getY() / 2);

		if (2 <= mousePos.getX() && mousePos.getX() <= 22 && -3 <= mousePos.getY() && mousePos.getY() <= 6) {
			mExpandOver = true;
		} else {
			mExpandOver = mExpandDown = false;
		}
		
		mousePos = evt.getMousePos() - mPos;

		if (mExpand && mousePos.getY() >= mSize.getY()) {
			Font* ft = Resources::inst().get<Font> (mFontName);
			float y = evt.getMousePos().getY() - mPos.getY() - mSize.getY();
			mHoverItem = y / (ft->getHeight() + 3);
			mOverItem = 0 <= mHoverItem && mHoverItem < mItems.size();
		} else {
			mOverItem = false;
		}
	}

	// ------------------------------------------------------------------
	void GUIComboBox::onMouseDown(GUIEvent& evt)
	{
		Vec2 mousePos = evt.getMousePos() - mPos;
		mousePos.setX(mousePos.getX() - mSize.getX() + 30);
		mousePos.setY(mousePos.getY() - mSize.getY() / 2);

		mExpandDown = (2 <= mousePos.getX() && mousePos.getX() <= 22 && -3 <= mousePos.getY() && mousePos.getY() <= 6);
	}

	// ------------------------------------------------------------------
	void GUIComboBox::onMouseUp(GUIEvent& evt)
	{
		Vec2 mousePos = evt.getMousePos() - mPos;
		mousePos.setX(mousePos.getX() - mSize.getX() + 30);
		mousePos.setY(mousePos.getY() - mSize.getY() / 2);

		if (2 <= mousePos.getX() && mousePos.getX() <= 22 && -3 <= mousePos.getY() && mousePos.getY() <= 6) {
			mExpandDown = false;
			mExpand = !mExpand;
		}

		if (mOverItem) {
			mSelectedItem = mHoverItem;
		}
	}

	// ------------------------------------------------------------------
	void GUIComboBox::onMouseLeave(GUIEvent& evt)
	{
		mOverItem = mExpandOver = mExpandDown = mExpand = false;
	}
	
	// ------------------------------------------------------------------
	bool GUIComboBox::isUnderMouse(Vec2 pos)
	{		
		if (!mExpand) {
			return (mPos < pos && pos < mPos + mSize);
		}

		Font* ft = Resources::inst().get<Font> (mFontName);
		unsigned mHeight = mItems.size() * (ft->getHeight() + 3) + 5;
		return (mPos < pos && pos < mPos + Vec2(mSize.getX(), mSize.getY() + mHeight));
	}
};