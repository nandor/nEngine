/**
	@file GUIElment.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIElement.hpp"

namespace nEngine {

	// ------------------------------------------------------------------
	GUIElement::GUIElement(const std::string& id)
		:mSize(150, 150),
		 mPos(0, 0),
		 mFontName("mono8"),
		 mBackgroundColor(0.7f, 0.7f, 0.7f),
		 mFontColor(0.0f, 0.0f, 0.0f),
		 mMouseOver(false),
		 mMousePressed(false),
		 mSizer(NULL),
		 mIsVisible(true)
	{
		mID = id;
	}
	
	// ------------------------------------------------------------------
	GUIElement::~GUIElement()
	{
		if (mSizer != NULL) {
			delete mSizer;
		}

		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			delete it->second;
		}
	}
	

	// ------------------------------------------------------------------
	void GUIElement::add(GUIElement* child)
	{
		if (!child) {
			throw Error("GUI", getID(), "Invalid child!");
		}

		if (!child->mSizer) {
			throw Error("GUI", child->getID(), "Element needs a sizer!");
		}

		tChildIter it = mChildren.find(child->getID());
		if (it == mChildren.end()) {
			child->mParent = this;

			mChildren.insert(make_pair(child->getID(), child));
		}
	}
	
	
	// ------------------------------------------------------------------
	void GUIElement::draw()
	{
		if (!mIsVisible)
			return;

		glPushMatrix();
		glTranslatef(mPos.getX(), mPos.getY(), 0.0f);

		onDraw();

		glPopMatrix();

		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			it->second->mSizer->applyTo(it->second);
			it->second->mSize = it->second->mSizer->getSize();
			it->second->mPos = it->second->mSizer->getPosition();

			it->second->draw();
		}

	}
	
	
	// ------------------------------------------------------------------
	void GUIElement::onDraw()
	{

	}


	// ------------------------------------------------------------------
	bool GUIElement::handleEvent(GUIEvent& evt)
	{
		if (!mIsVisible)
			return false;
		
		bool captured = true;
		if (!isUnderMouse(evt.getMousePos())) {
			if (evt.getType() != GUI_EVENT_MOUSEUP) {
				mMouseOver = false;
				mMousePressed = false;
			}
			captured = false;
		} else {
			switch (evt.getType()) {
			case GUI_EVENT_MOUSEMOVE:
				mMouseOver = true;
				break;
			case GUI_EVENT_MOUSEDOWN:
				mMousePressed = true;
				break;
			case GUI_EVENT_MOUSEUP:
				mMousePressed = false;
				fireEvent(GUIEvent(GUI_EVENT_CLICK, evt.getMousePos()));
				break;
			}
		}

		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			captured |= it->second->handleEvent(evt);
		}

		return captured;
	}
	
	
	// ------------------------------------------------------------------
	bool GUIElement::isUnderMouse(Vec2 pos)
	{
		if (mPos < pos && pos < mPos + mSize) {
			return true;
		} else {
			return false;
		}
	}
	
	
	// ------------------------------------------------------------------
	void GUIElement::setSizer(GUISizer* sizer)
	{
		if (mSizer) {
			delete mSizer;
		}

		mSizer = sizer;
	}
		
	// ------------------------------------------------------------------
	void GUIElement::connect(GUIEventType type, boost::function<void(GUIEvent&)> func)
	{
		if (0 <= type && type < GUI_NUM_EVENTS) {
			mEvents[type].push_back(func);
		} else {
			throw Error("GUI", getID(), "Attempting to connect an invalid event handler!");
		}
	}
	
	
	// ------------------------------------------------------------------
	void GUIElement::fireEvent(GUIEvent& evt)
	{
		for (tEventIter it = mEvents[evt.getType()].begin(); it != mEvents[evt.getType()].end(); ++it) {
			(*it) (evt);
		}
	}
};
