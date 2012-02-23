/**
	@file GUIPanel.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIPanel.hpp"
#include "GUI.hpp"

namespace nEngine {
	
	
	// -------------------------------------------------------------------------
	GUIPanel::GUIPanel(const std::string& id)
		:GUIElement(id)
	{

	}
	
	
	// -------------------------------------------------------------------------
	GUIPanel::~GUIPanel()
	{

	}
	
	
	// -------------------------------------------------------------------------
	void GUIPanel::onDraw()
	{		
		mBackgroundColor.glUse();

		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(mSize.getX(), 0);
			glVertex2i(mSize.getX(), mSize.getY());
			glVertex2i(0, mSize.getY());
		glEnd();	
	}
	
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIPanel, new)
	{
		GUIPanel* pnl = new GUIPanel(luaL_checkstring(L, 1));
		luaInstance(L, GUIPanel, pnl);
		return 1;
	}

	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIPanel, __setter)
	{
		GUIPanel* pnl = *(GUIPanel**)luaGetInstance(L, 1, "GUIPanel");

		lua_pushboolean(L, 0);
		return 1;
	}
	
	// -------------------------------------------------------------------------
	luaDeclareMethod(GUIPanel, get)
	{
		GUIPanel* panel = (GUIPanel*)GUI::inst().get(luaL_checkstring(L, 1));
		luaInstance(L, GUIPanel, panel);
		return 1;
	}

	// -------------------------------------------------------------------------
	luaBeginMeta(GUIPanel)
		luaMethod(GUIPanel, __setter)
	luaEndMeta()

	luaBeginMethods(GUIPanel)
		luaMethod(GUIPanel, new)
		luaMethod(GUIPanel, get)
	luaEndMethods()
	
	// -------------------------------------------------------------------------
	bool luaRegisterGUIPanel(lua_State* L)
	{
		luaInheritClass(L, GUIPanel, GUIElement);
		return true;
	}
};