/**
	@file MainNotebook.cpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEditor.hpp"
#include "MainNotebook.hpp"

BEGIN_EVENT_TABLE(MainNotebook, wxAuiNotebook)
	EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MainNotebook::OnTabClosing)
END_EVENT_TABLE()

// ------------------------------------------------------------------
MainNotebook::MainNotebook(wxWindow* parent, wxWindow* root)
	:wxAuiNotebook(parent, wxID_ANY, wxDefaultPosition, wxSize(300, 456), wxAUI_NB_CLOSE_ON_ACTIVE_TAB),
	 mMapEditor(NULL)
{
	mRoot = root;
}

// ------------------------------------------------------------------
MainNotebook::~MainNotebook()
{
	mFileEditors.clear();
}


// ------------------------------------------------------------------
void MainNotebook::AddMapEditor(MapEditor* mpEdit)
{
	if (mMapEditor != NULL) {
		mpEdit->Destroy();
		return;
	}

	this->AddPage(mpEdit, "Map Editor", true);
	this->Refresh();
	mMapEditor = mpEdit;
}


// ------------------------------------------------------------------
void MainNotebook::AddFileEditor(FileEditor* editor)
{
	for (tFileIter it = mFileEditors.begin(); it != mFileEditors.end(); ++it) {
		if ((*it)->GetFileName() == editor->GetFileName()) {
			editor->Destroy();
			return;
		}
	}

	mFileEditors.push_back(editor);
	this->AddPage(editor, editor->GetFileName(), true);
}

// ------------------------------------------------------------------
void MainNotebook::RemoveFileEditor(FileEditor* editor)
{
	for (tFileIter it = mFileEditors.begin(); it != mFileEditors.end(); ++it) {
		if ((*it)->GetFileName() == editor->GetFileName()) {
			mFileEditors.erase(it);
			return;
		}
	}
}

// ------------------------------------------------------------------
void MainNotebook::OnTabClosing(wxAuiNotebookEvent& evt)
{
	wxWindow* page = this->GetPage(evt.GetSelection());

	// Don't close the map editor
	if (page == mMapEditor) {
		evt.Veto();
		return;
	}

	wxMessageDialog* dlg = new wxMessageDialog(this, "Do you want to save the file?", "Save", wxYES_NO);
	FileEditor* fileEditor = (FileEditor*)page;

	if (fileEditor->IsModified()) {
		int result = dlg->ShowModal();
		
		if (result == wxID_YES) {	
			fileEditor->Save();	
		}
	}
	
	this->RemoveFileEditor(fileEditor);
}

// ------------------------------------------------------------------
void MainNotebook::SaveAll()
{
	for (tFileIter it = mFileEditors.begin(); it != mFileEditors.end(); ++it) {
		(*it)->Save();
	}
	mMapEditor->Save();
}