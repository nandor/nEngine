/**
	@file GUIGLCanvas.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIGLCanvas.hpp"
#include "GUI.hpp"

namespace nEngine {
	
	
	// ------------------------------------------------------------------
	GUIGLCanvas::GUIGLCanvas(const std::string& id)
		:GUIElement(id)
	{

	}
	
	
	// ------------------------------------------------------------------
	GUIGLCanvas::~GUIGLCanvas()
	{

	}
	
	
	// ------------------------------------------------------------------
	void GUIGLCanvas::onDraw()
	{

	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(GUIGLCanvas, new)
	{
		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(GUIGLCanvas)
	luaEndMeta()
	
	// ------------------------------------------------------------------
	luaBeginMethods(GUIGLCanvas)
		luaMethod(GUIGLCanvas, new)
	luaEndMethods()

	// ------------------------------------------------------------------
	bool luaRegisterGUIGLCanvas(lua_State* L)
	{
		luaInheritClass(L, GUIGLCanvas, GUIElement);
		return true;
	}
};