/**
	@file main.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEngine/nHeaders.hpp"
#include "nEngine/Lua.hpp"
#include "nEngine/Application.hpp"

#include <wx/wx.h>
#include <wx/datectrl.h>

#include "MainWindow.hpp"

class Application : public wxApp {
public:
    virtual bool OnInit()
	{
		try {

			nEngine::luaRegisterEngine();
			nEngine::luaReadFile("fs://data/lua/lib.lua");
			nEngine::luaReadFile("fs://data/lua/config.lua");
			
			MainWindow *wnd = new MainWindow(wxT("Simple"));	
			wnd->Show();

		} catch (nEngine::Error e) {
			wxMessageBox(wxT(e.getMessage()), "Error");
			return false;
		}
		
		return true;
	}
	
	virtual int OnExit()
	{
		nEngine::Application::killSingletons();
		return 0;
	}
};

IMPLEMENT_APP(Application)
