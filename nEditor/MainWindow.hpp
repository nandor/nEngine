/**
	@file MainWindow.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include <wx/wx.h>
#include <wx/notebook.h>

#include "MapEditor.hpp"
#include "TileList.hpp"
#include "ResourceView.hpp"
#include "PropertyEdit.hpp"

class MainWindow : public wxFrame {
public:
    
	MainWindow(const wxString& title);

	std::string GetMapName()
	{
		return mMapName;
	}

	bool Show(bool show = true);

	void OnResourceSelected(std::pair<std::string, nEngine::ResourceType> item);

private:

	void createMenu();

private:

	void OnQuit(wxCommandEvent& evt);
	void OnNew(wxCommandEvent& evt);
	void OnOpen(wxCommandEvent& evt);

private:

	ResourceView* mResourceView;
	PropertyEdit* mPropertyEdit;
	MapEditor* mMapEdit;
	TileList* mTileList;
	wxNotebook* mTabRight;

	std::string mMapName;
	bool mTileListEnabled;
};