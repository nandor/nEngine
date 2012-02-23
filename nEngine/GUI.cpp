/**
	@file GUI.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Util.hpp"
#include "Font.hpp"
#include "Shader.hpp"
#include "Resources.hpp"
#include "DataSource.hpp"

#include "GUI.hpp"
#include "GUIPanel.hpp"
#include "GUIButton.hpp"
#include "GUIGLCanvas.hpp"
#include "GUIStyledText.hpp"

namespace nEngine {		
	template<> GUI* GUI::Singleton<GUI>::__inst = NULL;

	// ------------------------------------------------------------------
	GUI::GUI()
		:GUIElement("")
	{
		mPos = mPos = Vec2(0, 0);
		mSize = mSize = Vec2(getScreenSize());
		mParent = NULL;
	}		
	
	// ------------------------------------------------------------------
	GUI::~GUI()
	{

	}	
	
	// ------------------------------------------------------------------
	void GUI::draw()
	{
		glPushMatrix();
		glDisable(GL_DEPTH_TEST);

		glLoadIdentity();
		
		Shader::useProgram("gui");
		GUIElement::draw();

		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}
	
	
	// ------------------------------------------------------------------
	bool GUI::handleEvent(GUIEvent& evt)
	{
		if (evt.getType() == GUI_EVENT_MOUSEMOVE) {
			mMousePos = evt.getMousePos();
		}

		bool captured = false;
		for (tChildIter it = mChildren.begin(); it != mChildren.end(); ++it) {
			captured |= it->second->handleEvent(evt);
		}

		return captured;
	}
	
	
	// ------------------------------------------------------------------
	bool GUI::addToGlobal(GUIElement* elem)
	{
		std::map<std::string, GUIElement*>::const_iterator it = mGlobal.find(elem->getID());

		if (it != mGlobal.end()) {
			return false;
		}

		mGlobal.insert(std::make_pair(elem->getID(), elem));
		return true;
	}

	// ------------------------------------------------------------------
	GUIElement* GUI::get(const std::string& id)
	{
		std::map<std::string, GUIElement*>::const_iterator it = mGlobal.find(id);

		if (it == mGlobal.end()) {
			throw Error("GUI", "Item does not exist: " + id);
		}

		return it->second;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(GUI, add)
	{
		GUIElement* elem = *(GUIElement**)luaGetInstance(L, 1, "GUIElement");
		GUI::inst().add(elem);
		return 0;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(GUI, loadScript)
	{
		GUI::inst().loadScript(luaL_checkstring(L, 1));
		return 1;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(GUI)
		
	luaEndMeta()
	
	// ------------------------------------------------------------------
	luaBeginMethods(GUI)
		luaMethod(GUI, add)
		luaMethod(GUI, loadScript)
	luaEndMethods()
	
	// ------------------------------------------------------------------
	bool luaRegisterGUI(lua_State* L)
	{
		luaClass(L, GUI);
		
		luaRegisterGUIEvent(L);
		luaRegisterGUIElement(L);
		luaRegisterGUIPanel(L);
		luaRegisterGUIGLCanvas(L);
		luaRegisterGUIStyledText(L);
		luaRegisterGUIButton(L);

		return true;
	}
};