/**
	@file GUIButton.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUISlider.hpp"
#include "GUI.hpp"

namespace nEngine {
	
	
	// ------------------------------------------------------------------
	GUISlider::GUISlider(const std::string& id)
		:GUIElement(id),
		 mValue(50),
		 mSliderColor(0.5f, 0.5f, 0.5f, 0.9f),
		 mSliderDown(false)
	{
		this->connect(GUI_EVENT_MOUSEDOWN, boost::bind(&GUISlider::onMouseDown, this, _1));
		this->connect(GUI_EVENT_MOUSEUP, boost::bind(&GUISlider::onMouseUp, this, _1));
		this->connect(GUI_EVENT_MOUSEMOVE, boost::bind(&GUISlider::onMouseMove, this, _1));
	}
	
	
	// ------------------------------------------------------------------
	GUISlider::~GUISlider()
	{

	}
	
	
	// ------------------------------------------------------------------
	void GUISlider::onDraw()
	{
		glTranslatef(0.0f, mSize.getY() / 2, 0.0f);

		glColor4f(0.5f, 0.5f, 0.5f, 0.9f);		
		glBegin(GL_QUADS);
			glVertex2i(0, -3);
			glVertex2i(mSize.getX(), -3);
			glVertex2i(mSize.getX(), 3);
			glVertex2i(0, 3);
		glEnd();

		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 3);
			glVertex2i(0, -3);
			glVertex2i(mSize.getX(), -3);
			glVertex2i(mSize.getX(), 3);
			glVertex2i(0, 3);
		glEnd();
		
		float ColorDiff = (mSliderDown ? 0.7 : (mMouseOver ? 1.2 : 1.0));
		(mSliderColor * ColorDiff).setA(1.0f).glUse();	

		glTranslatef(mSize.getX() * mValue / 100.0f, 0.0f, 0.0f);		
		glBegin(GL_QUADS);
			glVertex2i(-10, -15);
			glVertex2i(10, -15);
			glVertex2i(10, 15);
			glVertex2i(-10, 15);
		glEnd();	

		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2i(-10, 15);
			glVertex2i(-10, -15);
			glVertex2i(10, -15);
			glVertex2i(10, 15);
			glVertex2i(-10, 15);
		glEnd();
	}

	// ------------------------------------------------------------------
	void GUISlider::onMouseDown(GUIEvent& evt)
	{
		Vec2 pos = evt.getMousePos() - mPos - Vec2(mSize.getX() * mValue / 100.0f, mSize.getY() / 2);
		if (Vec2(-8, -15) < pos && pos < Vec2(8, 15)) {
			mSliderDown = true;
		}
	}

	// ------------------------------------------------------------------
	void GUISlider::onMouseMove(GUIEvent& evt)
	{
		Vec2 pos = evt.getMousePos() - mPos - Vec2(mSize.getX() * mValue / 100.0f, mSize.getY() / 2);
		if (Vec2(-8, -15) > pos || pos > Vec2(8, 15)) {
			return;
		}

		if (mSliderDown) {
			int x = evt.getMousePos().getX() - mPos.getX();
			mValue = x * 100.0f / mSize.getX();
			fireEvent(GUIEvent(GUI_EVENT_CHANGED));
		}
	}

	// ------------------------------------------------------------------
	void GUISlider::onMouseUp(GUIEvent& evt)
	{
		mSliderDown = false;
	}
};