/**
	@file GUIElment.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIElement.hpp"
#include "GUIScriptReader.hpp"
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
		 mEnabled(true)
	{
		mID = id;
	}
	
	// ------------------------------------------------------------------
	GUIElement::~GUIElement()
	{
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
	
	
	// -------------------------------------------------------------------------
	void GUIElement::draw()
	{
		if (!mIsVisible)
			return;

		glPushMatrix();
		glTranslatef(mPos.getX(), mPos.getY(), 0.0f);

		onDraw();

		glPopMatrix();

		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			it->second->computeSize();
			it->second->draw();
		}

	}
	
	
	// -------------------------------------------------------------------------
	void GUIElement::onDraw()
	{

	}
	
	// -------------------------------------------------------------------------
	void GUIElement::computeSize()
	{
		if (!this->getParentNode()) {
			throw Error ("GUISizer", "An element with a parent is required!");
		}

		GUIElement* parent = this->getParentNode();

		Vec2 parentSize = parent->getSize();
		Vec2 parentPos = parent->getPosition();

		if (mID == "showPanel") {
			int a = 5;
		}

		mSize.setX(mWidth.getMetric(parentSize.getX()));
		mSize.setY(mHeight.getMetric(parentSize.getY()));
		
		mPos.setX(mPosX.getMetric(parentSize.getX()));
		mPos.setY(mPosY.getMetric(parentSize.getY()));

		switch (mHorzAlign) {
		case GUI_ALIGN_LEFT:
		case GUI_ALIGN_NONE:
			mPos.setX(parentPos.getX() + mPos.getX());
			break;
		case GUI_ALIGN_RIGHT:
			mPos.setX(parentPos.getX() + parentSize.getX() - mSize.getX() - mPos.getX());
			break;
		case GUI_ALIGN_CENTER:
			mPos.setX(parentPos.getX() + (parentSize.getX() - mSize.getX()) / 2);
			break;
		default:
			mPos.setX(parentPos.getX());
			break;
		}

		switch (mVertAlign) {
		case GUI_ALIGN_TOP:
		case GUI_ALIGN_NONE:
			mPos.setY(parentPos.getY() + mPos.getY());
			break;
		case GUI_ALIGN_BOTTOM:
			mPos.setY(parentPos.getY() + parentSize.getY() - mSize.getY() - mPos.getY());
			break;
		case GUI_ALIGN_CENTER:
			mPos.setY(parentPos.getY() + (parentSize.getY() - mSize.getY()) / 2);
			break;
		default:
			mPos.setY(parentPos.getY());
			break;
		}
	}
	
	// -------------------------------------------------------------------------
	bool GUIElement::handleEvent(GUIEvent& evt)
	{
		if (!mIsVisible)
			return false;
		
		if (!mEnabled) {
			return isUnderMouse(evt.getMousePos());
		}

		bool captured = true;
		if (!isUnderMouse(evt.getMousePos())) {
			if (evt.getType() != GUI_EVENT_MOUSEUP) {
				mMouseOver = false;
				mMousePressed = false;
			}
			fireEvent(GUIEvent(GUI_EVENT_MOUSELEAVE, evt.getMousePos()));
			captured = false;
		} else {
			switch (evt.getType()) {
			case GUI_EVENT_MOUSEMOVE:
				if (!mMouseOver) {
					fireEvent(GUIEvent(GUI_EVENT_MOUSEENTER, evt.getMousePos()));
				} else {
					fireEvent(GUIEvent(GUI_EVENT_MOUSEMOVE, evt.getMousePos()));
				}
				mMouseOver = true;
				break;
			case GUI_EVENT_MOUSEDOWN:
				fireEvent(GUIEvent(GUI_EVENT_MOUSEDOWN, evt.getMousePos()));
				mMousePressed = true;
				break;
			case GUI_EVENT_MOUSEUP:
				mMousePressed = false;
				fireEvent(GUIEvent(GUI_EVENT_MOUSEUP, evt.getMousePos()));
				fireEvent(GUIEvent(GUI_EVENT_CLICK, evt.getMousePos()));
				break;
			}
		}

		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			captured |= it->second->handleEvent(evt);
		}

		return captured;
	}
	
	// -------------------------------------------------------------------------
	bool GUIElement::isUnderMouse(Vec2 pos)
	{
		if (mPos < pos && pos < mPos + mSize) {
			return true;
		}

		return false;
	}

	// -------------------------------------------------------------------------
	void GUIElement::connect(GUIEventType type, boost::function<void(GUIEvent&)> func)
	{
		if (0 <= type && type < GUI_NUM_EVENTS) {
			mEvents[type].push_back(func);
		} else {
			throw Error("GUI", getID(), "Attempting to connect an invalid event handler!");
		}
	}
	
	
	// -------------------------------------------------------------------------
	void GUIElement::fireEvent(GUIEvent& evt)
	{
		for (tEventIter it = mEvents[evt.getType()].begin(); it != mEvents[evt.getType()].end(); ++it) {
			if (*it != NULL) {
				(*it) (evt);
			}
		}
	}

	
	// -------------------------------------------------------------------------
	void GUIElement::remove()
	{
		if (mParent && mParent != this) {
			tChildIter it = mParent->mChildren.find(this->getID());

			if (it == mParent->mChildren.end()) {
				throw Error("GUIElement", getID(), "Invalid parent!");
			}
		
			mParent->mChildren.erase(it);
		}
	}	
	
	// ------------------------------------------------------------------
	void GUIElement::loadScript(const std::string& source)
	{
		DataSource* dataSource = Resources::inst().require<DataSource> (source);
		GUIScriptReader(dataSource, this).read();
	}
	
	// -------------------------------------------------------------------------
	GUIAlign getAlignment(const std::string& align)
	{
		if (align == "none") {
			return GUI_ALIGN_NONE;
		}

		if (align == "left") {
			return GUI_ALIGN_LEFT;
		}

		if (align == "top") {
			return GUI_ALIGN_TOP;
		}

		if (align == "bottom") {
			return GUI_ALIGN_BOTTOM;
		}

		if (align == "right") {
			return GUI_ALIGN_RIGHT;
		}

		if (align == "center") {
			return GUI_ALIGN_CENTER;
		}

		return GUI_ALIGN_NONE;
	}

	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, add)
	{
		GUIElement* parent = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		GUIElement* child = *(GUIElement**)luaGetInstance(L, 2, "GUIElement");

		parent->add(child);
		return 0;
	}
		
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, __setter)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		std::string field(luaL_checkstring(L, 2));
		
		if (field == "font") {
			elem->setFont(luaL_checkstring(L, 3));
			
		}

		if (field == "background") {
			Color* c = *(Color**)luaGetInstance(L, 3, "Color");
			elem->setBackgroundColor(*c);
			return 0;
		}

		return 0;
	}
	
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, setAlignment)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		
		elem->setAlignment(
			(GUIAlign)luaL_checkinteger(L, 2),
			(GUIAlign)luaL_checkinteger(L, 3)
		);

		return 0;
	}

	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, setSize)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		
		elem->setSize(
			GUIMetric(luaL_checkstring(L, 2)), 
			GUIMetric(luaL_checkstring(L, 3))
		);

		return 0;
	}
	

	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, setPosition)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		
		elem->setPosition(
			GUIMetric(luaL_checkstring(L, 2)), 
			GUIMetric(luaL_checkstring(L, 3))
		);

		return 0;
	}

	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, setVisibility)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		elem->setVisibility(lua_toboolean(L, 2));
		return 0;
	}
	
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, isVisible)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		lua_pushboolean(L, elem->isVisible());
		return 1;
	}

	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, connect)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		GUIEventType type = (GUIEventType)luaL_checkint(L, 2);
		
		if (!lua_isfunction(L, 3)) {
			throw Error ("GUIelement", elem->getID(), "[Lua] Not a c function!");
		}

		lua_pushvalue(L, 3);
		GUILuaHandler handler(luaL_ref(L, LUA_REGISTRYINDEX));
		elem->connect(type, handler);

		return 0;
	}
	
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIElement, loadScript)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		elem->loadScript(luaL_checkstring(L, 2));
		return 0;
	}

	// -------------------------------------------------------------------------
	luaBeginMeta(GUIElement)
		luaMethod(GUIElement, add)
		luaMethod(GUIElement, setSize)
		luaMethod(GUIElement, setPosition)
		luaMethod(GUIElement, setAlignment)
		luaMethod(GUIElement, setVisibility)
		luaMethod(GUIElement, isVisible)
		luaMethod(GUIElement, loadScript)
		luaMethod(GUIElement, connect)
		luaMethod(GUIElement, __setter)
	luaEndMeta()
	
	luaBeginMethods(GUIElement)
		
	luaEndMethods()

	// -------------------------------------------------------------------------
	bool luaRegisterGUIElement(lua_State* L)
	{
		luaClass(L, GUIElement);

		std::vector<std::pair<std::string, GUIAlign> > alignment;
		alignment.push_back(std::make_pair("GUI_ALIGN_TOP", GUI_ALIGN_TOP));
		alignment.push_back(std::make_pair("GUI_ALIGN_BOTTOM", GUI_ALIGN_BOTTOM));
		alignment.push_back(std::make_pair("GUI_ALIGN_CENTER", GUI_ALIGN_CENTER));
		alignment.push_back(std::make_pair("GUI_ALIGN_LEFT", GUI_ALIGN_LEFT));
		alignment.push_back(std::make_pair("GUI_ALIGN_RIGHT", GUI_ALIGN_RIGHT));
		alignment.push_back(std::make_pair("GUI_ALIGN_NONE", GUI_ALIGN_NONE));

		for (unsigned i = 0; i < alignment.size(); ++i)
		{
			lua_pushinteger(L, alignment[i].second);
			lua_setglobal(L, alignment[i].first.c_str());
		}
		
		return true;
	}
};
