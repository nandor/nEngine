/**
	@file Runner.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <wx/wx.h>

DECLARE_EVENT_TYPE(wxEVT_RUNEND, -1);

class Runner : public wxThread  {
public:
	/**
		Creates a new runner thread
		@param wnd			Pointer to parent window
		@param proj			Pointer to project
	*/
	Runner(wxWindow* wnd);

	/**
		Destroy the thread
	*/
	~Runner();
	
private:

	wxThread::ExitCode Entry();

private:
	
	wxWindow* mRoot;
};

#endif /*RUNNER_HPP*/