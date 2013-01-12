/**
	@file Runner.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEditor.hpp"
#include "Runner.hpp"
#include "MainWindow.hpp"
#include "nProj.hpp"

namespace fs = boost::filesystem3;

DEFINE_EVENT_TYPE(wxEVT_RUNEND)

// ------------------------------------------------------------------
Runner::Runner(wxWindow* wnd)
	:wxThread(wxTHREAD_DETACHED)
{
	mRoot = wnd;
	nProj* prj = ((MainWindow*)mRoot)->GetProject();
	
	mProjDir = prj->getDir();
	mProjName = prj->getProjName();

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
	try {
		run();
	} catch (std::exception e) {
		wxMessageBox("Unhandler exception: " + std::string(e.what()), "Error");
	} 

	return NULL;
}
// ------------------------------------------------------------------
void Runner::run()
{
	std::string executable(nEngine::luaGetGlobalString("execPath"));
	std::string params(executable + " " + nEngine::luaGetGlobalString("execArgs") + " ");

	fs::path dataDir(executable);
	dataDir.remove_filename();
	dataDir /= "data\\" + mProjName;

	fs::path projDir(mProjDir);
	projDir /= "build";

	if (!copyData(projDir, dataDir)) {
		throw std::logic_error("Cannot copy folders!");
	}

	// Add current directory
	params += "--init \"fs://data/" + mProjName + "/init.lua\"";

    STARTUPINFOA startupInfo; 
    PROCESS_INFORMATION processInfo; 
    memset(&startupInfo, 0, sizeof(startupInfo)); 
    memset(&processInfo, 0, sizeof(processInfo)); 
    startupInfo.cb = sizeof(processInfo); 
	
	char* paramPtr = new char[params.size() + 1];
	memcpy(paramPtr, params.c_str(), params.size());
	paramPtr[params.size()] = '\0';
	
	wxCommandEvent evt(wxEVT_RUNEND, wxID_ANY);

	if (CreateProcessA(executable.c_str(), paramPtr, 0, 0, false, CREATE_DEFAULT_ERROR_MODE, 0, 0, &startupInfo, &processInfo) != false) { 
        DWORD dwExitCode = WaitForSingleObject(processInfo.hProcess, INFINITE); 
		evt.SetInt(1);
		evt.SetString("Success!");
    } else { 
		evt.SetInt(0);
		evt.SetString("Cannot create child process : '" + nEngine::getWin32Error() + "'!");
    }
	
	if (fs::exists(dataDir)) {
		fs::remove_all(dataDir);
	}

	mRoot->GetEventHandler()->ProcessEvent(evt);
	delete[] paramPtr;	
}

// ------------------------------------------------------------------
bool Runner::copyData(fs::path source, fs::path dest)
{
	if (!fs::exists(source)) {
		return false;
	}
	
	if (fs::exists(dest)) {
		fs::remove_all(dest);
	}

	fs::create_directories(dest);
	for (fs::directory_iterator it(source); it != fs::directory_iterator(); ++it) {
		if (fs::is_regular_file(*it)) {
			fs::copy_file(it->path(), dest / it->path().filename());
		}
	}
	
	return true;
}