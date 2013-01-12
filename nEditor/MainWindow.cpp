/**
	@file MainWindow.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEditor.hpp"
#include "nEngine/Resources.hpp"
#include "nEngine/Map.hpp"
#include "nEngine/Lua.hpp"

#include "Icons/blend.xpm"
#include "Icons/delete.xpm"
#include "Icons/place.xpm"
#include "Icons/tileEdit.xpm"

#include "MainWindow.hpp"
#include "Runner.hpp"

enum wxMenuId {
	MENU_FILE_NEW = wxID_NEW,
	MENU_FILE_SAVE = wxID_SAVE,
	MENU_FILE_OPEN = wxID_OPEN,
	MENU_FILE_QUIT = wxID_CLOSE,

	MENU_VIEW_MAPEDIT = 0,

	MENU_BUILD_COMPILE,
	MENU_BUILD_CLEAN,
	MENU_BUILD_RUN,

	TOOLBAR,
	TOOLBAR_BLEND,
	TOOLBAR_MOVE,
	TOOLBAR_PLACE,
	TOOLBAR_TILE,
};

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
	EVT_TOOL_RANGE(TOOLBAR_BLEND, TOOLBAR_PLACE, MainWindow::OnToolbar)
	EVT_COMMAND(wxID_ANY, wxEVT_COMPILE, MainWindow::OnCompileMessage)
	EVT_COMMAND(wxID_ANY, wxEVT_RUNEND, MainWindow::OnRunMessage)
END_EVENT_TABLE()

// ------------------------------------------------------------------
MainWindow::MainWindow(const wxString& title)
       :wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 150)),
	   mTileListEnabled(false),
	   mProj(NULL),
	   mProgressDialog(NULL)
{
	CreateMenu();
	CreateToolbar();

	wxSizer* horzSizer = new wxBoxSizer(wxHORIZONTAL);

	// Tabbed view on the right
	mTabRight = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(200, 456));
	horzSizer->Add(mTabRight, 0, wxEXPAND);

	// Resource view on the first tab
	mResourceView = new ResourceView(mTabRight, wxSize(200, 456), this);
	mTabRight->AddPage(mResourceView, "Resource view");
	
	// Tile view on the second tab
	mTileList = new TileList(mTabRight, wxSize(200, 456), this);
	mTabRight->AddPage(mTileList, "Tiles");

	// Tab view in the middle
	mMainNotebook = new MainNotebook(this, this);
	horzSizer->Add(mMainNotebook, 1, wxEXPAND);
	
	// Main map editor
	mMapEdit = new MapEditor(mMainNotebook, wxSize(400, 456), this);
	mMainNotebook->AddMapEditor(mMapEdit);

	// Property editor
	mPropertyEdit = new PropertyEdit(this, wxSize(200, 456), this);
	horzSizer->Add(mPropertyEdit, 0, wxEXPAND);
	horzSizer->Show (true);
	
	this->SetSizerAndFit(horzSizer);
	this->SetSize(768, 456);
}


// ------------------------------------------------------------------
MainWindow::~MainWindow()
{
	if (mProj) {
		delete mProj;
	}
}

// ------------------------------------------------------------------
void MainWindow::CreateMenu()
{
	// Menu
	wxMenuBar* menuBar = new wxMenuBar();

	// Menu -> File
	wxMenu* fileMenu = new wxMenu();
	menuBar->Append(fileMenu, wxT("&File"));

	// Menu -> File -> New
	fileMenu->Append(MENU_FILE_NEW, wxT("&New Project"));
	this->Connect(MENU_FILE_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnNew));

	// Menu -> File -> Open
	fileMenu->Append(MENU_FILE_OPEN, wxT("&Open Project"));
	this->Connect(MENU_FILE_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnOpen));
	
	// Menu -> File -> Save
	fileMenu->Append(MENU_FILE_SAVE, wxT("&Save"));
	this->Connect(MENU_FILE_SAVE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnSave));

	// Menu -> File -> Quit
	fileMenu->Append(MENU_FILE_QUIT, wxT("&Quit"));
	this->Connect(MENU_FILE_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnQuit));

	// Menu -> Edit
	wxMenu* editMenu = new wxMenu();
	menuBar->Append(editMenu, wxT("&Edit"));
	
	// Menu -> Build
	wxMenu* buildMenu = new wxMenu();
	menuBar->Append(buildMenu, wxT("&Build"));

	// Menu -> Build -> Compile
	buildMenu->Append(MENU_BUILD_COMPILE, wxT("&Compile"));
	this->Connect(MENU_BUILD_COMPILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnCompile));

	buildMenu->Append(MENU_BUILD_RUN, wxT("&Run"));
	this->Connect(MENU_BUILD_RUN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnRun));

	// Menu -> Help
	wxMenu* helpMenu = new wxMenu();
	menuBar->Append(helpMenu, wxT("&Help"));

	this->SetMenuBar(menuBar);
}

// ------------------------------------------------------------------
void MainWindow::CreateToolbar()
{
	wxToolBar* toolBar = this->CreateToolBar(wxNO_BORDER|wxHORIZONTAL|wxTB_FLAT, TOOLBAR);
	 
	toolBar->AddTool(TOOLBAR_PLACE, "Place", wxIcon(place_xpm)); 
	toolBar->AddTool(TOOLBAR_BLEND, "Blend", wxIcon(blend_xpm)); 
	toolBar->AddTool(TOOLBAR_TILE,  "Edit Tile", wxIcon(tileEdit_xpm)); 
	toolBar->Realize();
}

// ------------------------------------------------------------------
void MainWindow::OnQuit(wxCommandEvent&)
{
	this->Close(true);
}

// ------------------------------------------------------------------
void MainWindow::OnNew(wxCommandEvent&)
{
	wxDirDialog* newDlg = new wxDirDialog(this,
		"Chose a folder for the project",
		"D:\\test project\\");
 
	if (newDlg->ShowModal() == wxID_OK)	{
		std::string directory = newDlg->GetPath();

		nProj* newProj = NULL;

		try {
			newProj = new nProj(directory);
		} catch (nEngine::Error e) {
			wxMessageBox(e.getMessage(), "Error");
		}

		if (newProj != NULL) {
			mProj = newProj;
			
			mMapEdit->Refresh();
			mResourceView->Refresh();
			mResourceView->Refresh();
		}

		SetTitle("nEditor - " + directory);
	}
 
	newDlg->Destroy();
}

// ------------------------------------------------------------------
void MainWindow::OnSave(wxCommandEvent&)
{
	mMainNotebook->SaveAll();
	mProj ? mProj->save() : NULL;
}

// ------------------------------------------------------------------
void MainWindow::OnCompile(wxCommandEvent&)
{
	if (mProj) {
		nEngine::File::setenv("path", mProj->getDir() + "/src");
		mMainNotebook->SaveAll();

		mProgressDialog = new wxProgressDialog(
			"Compiling '" + mProj->getName() + "'", 
			"Starting...",
			100, this, 
			wxPD_APP_MODAL | wxPD_ESTIMATED_TIME);
		mProgressDialog->SetSize(300, 150);
		new ResourceCompiler(this, mProj);
	}
}

// ------------------------------------------------------------------
void MainWindow::OnRun(wxCommandEvent&)
{
	this->Disable();
	new Runner(this);
}

// ------------------------------------------------------------------
void MainWindow::OnRunMessage(wxCommandEvent& evt)
{
	this->Enable();
	if (!evt.GetInt()) {
		wxMessageBox(evt.GetString(), "Error!");
	}
}

// ------------------------------------------------------------------
void MainWindow::OnCompileMessage(wxCommandEvent& evt)
{
	if (mProgressDialog) {
		int progress = evt.GetInt();
		if (progress == 100) {
			delete mProgressDialog;
			mProgressDialog = NULL;
		} else {			
			mProgressDialog->Update(progress, evt.GetString());
		}
	}
}

// ------------------------------------------------------------------
void MainWindow::OnOpen(wxCommandEvent&)
{
	wxFileDialog* openDlg = new wxFileDialog(this, 
		"Choose a project file", wxEmptyString, wxEmptyString, 
		"NPROJ Files (*.nproj)|*.nproj|All Files (*.*)|*.*", 
		wxFD_OPEN, wxDefaultPosition);
 
	if (openDlg->ShowModal() == wxID_OK)	{
		std::string filePath = openDlg->GetPath();
		std::string directory = openDlg->GetDirectory();

		nProj* newProj = NULL;

		try {
			newProj = new nProj(filePath, directory);
		} catch (nEngine::Error e) {
			wxMessageBox(e.getMessage(), "Error");
		}

		if (newProj != NULL) {
			mProj = newProj;
		
			mMapEdit->Refresh();
			mResourceView->Refresh();
			mResourceView->Refresh();
		}

		SetTitle("nEditor - " + filePath);
	}
 
	openDlg->Destroy();
}

// ------------------------------------------------------------------
void MainWindow::OnToolbar(wxCommandEvent& evt)
{
	switch (evt.GetId()) {
	case TOOLBAR_BLEND:
		break;
	case TOOLBAR_MOVE:
		break;
	case TOOLBAR_PLACE:
		SetEditMode(MAP_EDIT_PLACE);
		break;
	default:
		break;
	}
}

// ------------------------------------------------------------------
bool MainWindow::Show(bool show)
{
	wxFrame::Show(true);
	mMapEdit->InitOpenGL();	

	return true;
}

// ------------------------------------------------------------------
void MainWindow::OnResourceSelected(std::pair<std::string, nEngine::ResourceType> item)
{	
	switch (item.second) {
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

// ------------------------------------------------------------------
void MainWindow::SetField(nEngine::FieldType* f)
{
	mMapEdit->SetField(f);
}

// ------------------------------------------------------------------
void MainWindow::OpenFileEditor(const std::string& id)
{
	nEngine::File* toEdit = nEngine::Resources::inst().get<nEngine::File> (id);
	
	FileEditor* fileEditor = new FileEditor(mMainNotebook, toEdit);
	mMainNotebook->AddFileEditor(fileEditor);
}
