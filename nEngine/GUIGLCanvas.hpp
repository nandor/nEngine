/**
	@file GUIGLCanvas.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef GUIGLCANVAS_HPP
#define GUIGLCANVAS_HPP

#include "Singleton.hpp"
#include "GUIElement.hpp"

namespace nEngine {
	class GUIGLCanvas : public GUIElement  {
	public:
		NAPI GUIGLCanvas(const std::string& id);
		NAPI ~GUIGLCanvas();

		NAPI void onDraw();
	};

	bool luaRegisterGUIGLCanvas(lua_State* L);
};

#endif /*GUIGLCANVAS_HPP*/