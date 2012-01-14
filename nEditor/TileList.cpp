/**
	@file TileList.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEngine/Resources.hpp"
#include "nEngine/Map.hpp"

#include "TileList.hpp"
#include "MainWindow.hpp"

BEGIN_EVENT_TABLE(TileList, wxListCtrl)
	EVT_LIST_COL_BEGIN_DRAG(wxID_ANY, TileList::OnColResize)
END_EVENT_TABLE()

// ------------------------------------------------------------------
TileList::TileList(wxWindow* parent, wxSize size, wxWindow* root)
	:wxListCtrl(parent, wxID_ANY, wxDefaultPosition, size, wxLC_REPORT)
{
	mRoot = root;

	wxListItem imageCol;
	imageCol.SetId(0);
	imageCol.SetText("Tile");
	imageCol.SetWidth(60);
	this->InsertColumn(0, imageCol);

	wxListItem nameCol;
	nameCol.SetId(1);
	nameCol.SetText("Name");
	nameCol.SetWidth(size.GetX() - 73);
	this->InsertColumn(1, nameCol);
}


// ------------------------------------------------------------------
TileList::~TileList()
{

}

// ------------------------------------------------------------------
void TileList::Refresh()
{
	std::string mapName = ((MainWindow*)mRoot)->GetMapName();
	nEngine::Map* map = nEngine::Resources::inst().require<nEngine::Map> (mapName);

	this->DeleteAllItems();
}

// ------------------------------------------------------------------
void TileList::OnColResize(wxListEvent& evt)
{
	evt.Veto();
}