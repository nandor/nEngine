/**
	@file MainNotebook.hpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef MAINNOTEBOOK_HPP
#define MAINNOTEBOOK_HPP

#include "nEngine/nHeaders.hpp"

#include <wx/aui/auibook.h>
#include <wx/msgdlg.h>

#include "MapEditor.hpp"
#include "FileEditor.hpp"

class MainNotebook : public wxAuiNotebook {
public:
	MainNotebook(wxWindow* parent, wxWindow* root);
	~MainNotebook();

	void AddMapEditor(MapEditor* mapEditor);
	void AddFileEditor(FileEditor* fileEditor);
	void SaveAll();

private:
	
	DECLARE_EVENT_TABLE();
	void OnTabClosing(wxAuiNotebookEvent& evt);

private:
	
	void RemoveFileEditor(FileEditor* fileEditor);
	
private:
	
	std::vector<FileEditor*> mFileEditors;
	typedef std::vector<FileEditor*>::iterator tFileIter;
	MapEditor* mMapEditor;
	wxWindow* mRoot;
};

#endif /*MAINNOTEBOOK_HPP*/