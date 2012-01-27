/**
	@file FileEditor.hpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef FILEEDITOR_HPP
#define FILEEDITOR_HPP

#include "nEngine/nHeaders.hpp"
#include "nEngine/File.hpp"

#include <wx/wx.h>
#include <wx/wxscintilla.h>

class FileEditor : public wxScintilla {
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
	void OnModified(wxScintillaEvent& evt);

private:

	nEngine::File* mFile;
	bool mModified;
	bool mEditable;
};

#endif /*FILEEDITOR_HPP*/