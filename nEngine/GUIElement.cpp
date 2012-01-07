/**
	@file GUIElment.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIElement.hpp"
#include "GUI.hpp"

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
		 mIsVisible(true),
		 mVertAlign(GUI_ALIGN_NONE),
		 mHorzAlign(GUI_ALIGN_NONE),
		 mComputedPos(0, 0),
		 mComputedSize(0, 0),
		 mWidthType(GUI_SIZE_PIXEL),
		 mHeightType(GUI_SIZE_PIXEL)
	{
		mID = id;
	}
	
	// ------------------------------------------------------------------
	GUIElement::~GUIElement()
	{
		tChildIter it = mParent->mChildren.find(this->getID());

		if (it == mParent->mChildren.end()) {
			throw Error("GUIElement", getID(), "Invalid parent!");
		}
		
		mParent->mChildren.erase(it);

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
		
		if (!GUI::inst().addToGlobal(child)) {
			throw Error ("GUI", getID(), "An element with that ID already exists!");
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
		glTranslatef(mComputedPos.getX(), mComputedPos.getY(), 0.0f);

		onDraw();

		glPopMatrix();

		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			it->second->computeSize();
			it->second->draw();
		}

	}
	
	
	// ------------------------------------------------------------------
	void GUIElement::onDraw()
	{

	}


	// ------------------------------------------------------------------
	void GUIElement::computeSize()
	{
		if (!this->getParentNode()) {
			throw Error ("GUISizer", "An element with a parent is required!");
		}

		GUIElement* parent = this->getParentNode();

		Vec2 parentSize = parent->getSize();
		Vec2 parentPos = parent->getPosition();

		mComputedSize = mSize;
		if (mWidthType == GUI_SIZE_PERCENT) {
			mComputedSize.setX(parentSize.getX() * mSize.getX() / 100.0f);
		}

		if (mHeightType == GUI_SIZE_PERCENT) {
			mComputedSize.setY(parentSize.getY() * mSize.getY() / 100.0f);
		}


		mComputedPos = mPos;
		switch (mHorzAlign) {
		case GUI_ALIGN_LEFT:
			mComputedPos.setX(parentPos.getX() + mPos.getX());
			break;
		case GUI_ALIGN_RIGHT:
			mComputedPos.setX(parentSize.getX() - mComputedSize.getX() - mPos.getX());
			break;
		case GUI_ALIGN_CENTER:
			mComputedPos.setX((parentSize.getX() - mComputedSize.getX()) / 2);
			break;
		default:
			break;
		}

		switch (mVertAlign) {
		case GUI_ALIGN_TOP:
			mComputedPos.setY(parentPos.getY() + mPos.getY());
			break;
		case GUI_ALIGN_BOTTOM:
			mComputedPos.setY(parentSize.getY() - mComputedSize.getY() - mPos.getY());
			break;
		case GUI_ALIGN_CENTER:
			mComputedPos.setY((parentSize.getY() - mComputedSize.getY()) / 2);
			break;
		default:
			break;
		}

		mComputedPos += parent->getPosition();
	}
	

	// ------------------------------------------------------------------
	bool GUIElement::handleEvent(GUIEvent& evt)
	{
		if (!mIsVisible || !mEnabled)
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
		if (mComputedPos < pos && pos < mComputedPos + mSize) {
			return true;
		}

		return false;
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
			if (*it != NULL) {
				(*it) (evt);
			}
		}
	}
};
