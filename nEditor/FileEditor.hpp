/**
	@file FileEditor.hpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef FILEEDITOR_HPP
#define FILEEDITOR_HPP

#include "nEditor.hpp"
#include <wx/stc/stc.h>

class FileEditor : public wxStyledTextCtrl {
public:
	FileEditor(wxWindow* parent, nEngine::File* file);
	~FileEditor();

	bool IsModified()
	{
		return mEditable && mModified;
	}

	void Save();

	std::string GetFileName();

private:

	DECLARE_EVENT_TABLE();
	void OnModified(wxStyledTextEvent& evt);

private:

	nEngine::File* mFile;
	bool mModified;
	bool mEditable;
};

#endif /*FILEEDITOR_HPP*/