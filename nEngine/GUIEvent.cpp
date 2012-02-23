/**
	@file GUIEvent.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "GUIEvent.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	GUIEvent::GUIEvent()
	{

	}
	
	// ------------------------------------------------------------------
	GUIEvent::GUIEvent(GUIEventType type)
	{
		mType = type;
	}

	// ------------------------------------------------------------------
	GUIEvent::GUIEvent(GUIEventType type, int x, int y)
	{
		mType = type;
		mMouseX = x;
		mMouseY = y;
	}	
	
	// ------------------------------------------------------------------
	GUIEvent::GUIEvent(GUIEventType type, Vec2 mouse)
	{
		mType = type;
		mMouseX = mouse.getX();
		mMouseY = mouse.getY();
	}
		
	// ------------------------------------------------------------------
	GUIEvent::~GUIEvent ()
	{

	}
	
	// ------------------------------------------------------------------
	GUILuaHandler::GUILuaHandler(int ref)
	{
		mRef = ref;
	}

	// ------------------------------------------------------------------
	GUILuaHandler::~GUILuaHandler()
	{
	
	}

	// ------------------------------------------------------------------
	void GUILuaHandler::operator () (GUIEvent& evt)
	{
		GUIEvent* evtPtr = new GUIEvent(evt);
		lua_State* L = luaGlobalState();

		lua_rawgeti(L, LUA_REGISTRYINDEX, mRef);
		luaInstance(L, GUIEvent, evtPtr);
		lua_pcall(L, 1, 0, 0);
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(GUIEvent, __gc)
	{
		GUIEvent* evt = *(GUIEvent**)luaGetInstance(L, 1, "GUIEvent");
		if (evt != NULL) {
			delete evt;
		}
		
		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(GUIEvent)
		luaMethod(GUIEvent, __gc)
	luaEndMeta();

	luaBeginMethods(GUIEvent)
	luaEndMethods()
		
	// ------------------------------------------------------------------
	bool luaRegisterGUIEvent(lua_State* L)
	{
		luaClass(L, GUIEvent);

		std::vector<std::pair<std::string, GUIEventType> > events;
		events.push_back(std::make_pair("GUI_EVENT_MOUSEDOWN", GUI_EVENT_MOUSEDOWN));
		events.push_back(std::make_pair("GUI_EVENT_MOUSEUP", GUI_EVENT_MOUSEUP));
		events.push_back(std::make_pair("GUI_EVENT_MOUSEMOVE", GUI_EVENT_MOUSEMOVE));
		events.push_back(std::make_pair("GUI_EVENT_MOUSEENTER", GUI_EVENT_MOUSEENTER));
		events.push_back(std::make_pair("GUI_EVENT_MOUSELEAVE", GUI_EVENT_MOUSELEAVE));
		events.push_back(std::make_pair("GUI_EVENT_MOUSE", GUI_EVENT_MOUSE));
		events.push_back(std::make_pair("GUI_EVENT_CLICK", GUI_EVENT_CLICK));
		events.push_back(std::make_pair("GUI_EVENT_SELECTION", GUI_EVENT_SELECTION));
		events.push_back(std::make_pair("GUI_EVENT_CHANGED", GUI_EVENT_CHANGED));

		for (unsigned i = 0; i < events.size(); ++i)
		{
			lua_pushinteger(L, events[i].second);
			lua_setglobal(L, events[i].first.c_str());
		}
		return true;
	}
};