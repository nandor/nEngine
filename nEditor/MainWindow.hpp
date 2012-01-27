/**
	@file MainWindow.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/aui/auibook.h>

#include "nEngine/Map.hpp"

#include "nProj.hpp"
#include "TileList.hpp"
#include "ResourceView.hpp"
#include "PropertyEdit.hpp"
#include "MainNotebook.hpp"
#include "ResourceCompiler.hpp"

enum MapEditMode {
	MAP_EDIT_PLACE = 1,
	MAP_EDIT_DELETE
};

class MainWindow : public wxFrame {
public:
    
	MainWindow(const wxString& title);
	~MainWindow();

	std::string GetMapName()
	{
		return mMapName;
	}
	
	bool Show(bool show = true);

	void SetEditMode(int mapEditMode)
	{
		mEditMode = (MapEditMode)mapEditMode;
	}

	MapEditMode GetEditMode()
	{
		return mEditMode;
	}

	nProj* GetProject()
	{
		return mProj;
	}

	void OnResourceSelected(std::pair<std::string, nEngine::ResourceType> item);
	void SetField(nEngine::FieldType* f);
	void OpenFileEditor(const std::string& id);

private:

	void CreateMenu();
	void CreateToolbar();

private:

	DECLARE_EVENT_TABLE();

	void OnQuit(wxCommandEvent& evt);
	void OnNew(wxCommandEvent& evt);
	void OnOpen(wxCommandEvent& evt);
	void OnSave(wxCommandEvent& evt);
	void OnToolbar(wxCommandEvent& evt);

	void OnCompile(wxCommandEvent& evt);
	void OnRun(wxCommandEvent& evt);
	void OnKeyUp(wxKeyEvent& evt);

	void OnCompileMessage(wxCommandEvent& evt);
	void OnRunMessage(wxCommandEvent& evt);

private:

	ResourceView* mResourceView;
	PropertyEdit* mPropertyEdit;
	MapEditor* mMapEdit;
	TileList* mTileList;
	MapEditMode mEditMode;
	MainNotebook* mMainNotebook;
	wxProgressDialog* mProgressDialog;

	wxNotebook* mTabRight;

	nProj* mProj;
	std::string mMapName;
	bool mTileListEnabled;
};