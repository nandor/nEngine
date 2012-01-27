/**
	@file Runner.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "Runner.hpp"
#include "MainWindow.hpp"
#include "nProj.hpp"

DEFINE_EVENT_TYPE(wxEVT_RUNEND)

// ------------------------------------------------------------------
Runner::Runner(wxWindow* wnd)
	:wxThread(wxTHREAD_DETACHED)
{
	mRoot = wnd;
	nProj* prj = ((MainWindow*)mRoot)->GetProject();

	if (Create() == wxTHREAD_NO_ERROR) {
		wxThread::Run();
    }
}

// ------------------------------------------------------------------
Runner::~Runner()
{

}

// ------------------------------------------------------------------
wxThread::ExitCode Runner::Entry()
{
	std::string executable(nEngine::luaGetGlobalString("execPath"));
	std::string params(executable + " " + nEngine::luaGetGlobalString("execArgs"));

    STARTUPINFO startupInfo; 
    PROCESS_INFORMATION processInfo; 
    memset(&startupInfo, 0, sizeof(startupInfo)); 
    memset(&processInfo, 0, sizeof(processInfo)); 
    startupInfo.cb = sizeof(processInfo); 
	
	char* paramPtr = new char[params.size() + 1];
	memcpy(paramPtr, params.c_str(), params.size());
	paramPtr[params.size()] = '\0';
	
	wxCommandEvent evt(wxEVT_RUNEND, wxID_ANY);

	if (CreateProcess(executable.c_str(), paramPtr, 0, 0, false, CREATE_DEFAULT_ERROR_MODE, 0, 0, &startupInfo, &processInfo) != false) { 
        DWORD dwExitCode = WaitForSingleObject(processInfo.hProcess, INFINITE); 
		evt.SetInt(1);
		evt.SetString("Success!");
    } else { 
		evt.SetInt(0);
		evt.SetString("Cannot create child process : '" + nEngine::getWin32Error() + "'!");
    }

	mRoot->AddPendingEvent(evt);
	delete[] paramPtr;	
	
	return NULL;
}