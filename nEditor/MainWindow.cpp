/**
	@file MainWindow.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEngine/nHeaders.hpp"
#include "nEngine/Resources.hpp"

#include "MainWindow.hpp"

enum wxMenuId {
	MENU_FILE_NEW,
	MENU_FILE_OPEN,
	MENU_FILE_SAVE,
	MENU_FILE_QUIT
};

// ------------------------------------------------------------------
MainWindow::MainWindow(const wxString& title)
       :wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150)),
	   mTileListEnabled(false)
{
	createMenu();

	wxSizer* horzSizer = new wxBoxSizer(wxHORIZONTAL);

	// Tabbed view on the right
	mTabRight = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(200, 456));
	mTabRight->Show(true);
	horzSizer->Add(mTabRight, 0, wxEXPAND);

	// Resource view on the first tab
	mResourceView = new ResourceView(mTabRight, wxSize(200, 456), this);
	mResourceView->Show(true);
	mTabRight->AddPage(mResourceView, "Resource view");
	
	// Tile view on the second tab
	mTileList = new TileList(mTabRight, wxSize(200, 456), this);
	mTileList->Show(true);
	mTabRight->AddPage(mTileList, "Tiles");

	// Main map editor
	mMapEdit = new MapEditor(this, wxSize(400, 456));
	mMapEdit->Show(true);
	horzSizer->Add(mMapEdit, 1, wxEXPAND);

	// Property editor
	mPropertyEdit = new PropertyEdit(this, wxSize(200, 456), this);
	horzSizer->Add(mPropertyEdit, 0, wxEXPAND);
	horzSizer->Show (true);

	this->SetSizerAndFit(horzSizer);
}

// ------------------------------------------------------------------
void MainWindow::createMenu()
{
	// Menu
	wxMenuBar* menuBar = new wxMenuBar();

	// Menu -> File
	wxMenu* fileMenu = new wxMenu();
	menuBar->Append(fileMenu, wxT("&File"));

	// Menu -> File -> New
	fileMenu->Append(MENU_FILE_NEW, wxT("&New"));
	this->Connect(MENU_FILE_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnNew));

	// Menu -> File -> Open
	fileMenu->Append(MENU_FILE_OPEN, wxT("&Open"));
	this->Connect(MENU_FILE_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnOpen));

	// Menu -> File -> Quit
	fileMenu->Append(MENU_FILE_QUIT, wxT("&Quit"));
	this->Connect(MENU_FILE_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnQuit));

	this->SetMenuBar(menuBar);
}


// ------------------------------------------------------------------
void MainWindow::OnQuit(wxCommandEvent&)
{
	this->Close(true);
}


// ------------------------------------------------------------------
void MainWindow::OnNew(wxCommandEvent&)
{
	try {
		mMapEdit->SetCurrent();
		nEngine::Resources::inst().loadResourceGroup("core", "fs://data/core.json");
		mResourceView->Refresh();
	} catch (nEngine::Error e) {
		wxMessageBox(e.getMessage(), "Error");
		nEngine::Resources::inst().unloadResourceGroup("core");
	}
}

// ------------------------------------------------------------------
void MainWindow::OnOpen(wxCommandEvent&)
{

}


// ------------------------------------------------------------------
bool MainWindow::Show(bool show)
{
	wxFrame::Show(true);
	mMapEdit->InitOpenGL();
		
	// TEMP STUFF
	mMapEdit->SetCurrent();
	nEngine::Resources::inst().loadResourceGroup("core", "fs://data/core.json");
	mResourceView->Refresh();

	return true;
}


// ------------------------------------------------------------------
void MainWindow::OnResourceSelected(std::pair<std::string, nEngine::ResourceType> item)
{	
	switch (item.second) {
	case nEngine::RESOURCE_JSON:
		mPropertyEdit->setResourceJSON(item.first);
		break;

	case nEngine::RESOURCE_SHADER:
		mPropertyEdit->setResourceShader(item.first);
		break;

	case nEngine::RESOURCE_MAP:
		if (mMapName != item.first) {
			mMapName = item.first;
			mMapEdit->SetMap(mMapName);
			mTileList->Refresh();
		}
		break;

	default:
		break;
	}
}