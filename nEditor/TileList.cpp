/**
	@file TileList.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEditor.hpp"
#include "TileList.hpp"
#include "MainWindow.hpp"

BEGIN_EVENT_TABLE(TileList, wxListCtrl)
	EVT_LIST_COL_BEGIN_DRAG(wxID_ANY, TileList::OnColResize)
	EVT_LIST_ITEM_SELECTED(wxID_ANY, TileList::OnItemSelect)
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
	MainWindow* wnd = (MainWindow*)mRoot;
	std::string mapName = wnd->GetMapName();
	nEngine::Map* map = nEngine::Resources::inst().require<nEngine::Map> (mapName);

	std::map<int, nEngine::FieldType>& fields = map->getFieldTypes();
	std::map<int, nEngine::FieldType>::iterator it;
	int idx;

	this->DeleteAllItems();

	wnd->SetField(fields.empty() ? NULL : (&(fields.begin()->second)));

	wxImageList* imgList = new wxImageList(60, 30, true);

	for (it = fields.begin(), idx = 0; it != fields.end(); ++it, ++idx) {
		nEngine::File* imgData = nEngine::Resources::inst().require<nEngine::File> (it->second.mImage);
		wxMemoryInputStream is(imgData->getData(), imgData->getSize());

		wxBitmap image = wxBitmap(wxImage(is, wxBITMAP_TYPE_PNG).Rescale(60, 30, wxIMAGE_QUALITY_NORMAL));

		imgList->Add(image);
	}

	this->AssignImageList(imgList, wxIMAGE_LIST_SMALL);

	for (it = fields.begin(), idx = 0; it != fields.end(); ++it, ++idx) {
		wxListItem item;
		item.SetId(it->first);	
		this->InsertItem(item);

		this->SetItemImage(it->first, idx, idx);
		this->SetItem(it->first, 1, it->second.mName);
	}

}

// ------------------------------------------------------------------
void TileList::OnColResize(wxListEvent& evt)
{
	evt.Veto();
}

// ------------------------------------------------------------------
void TileList::OnItemSelect(wxListEvent& evt)
{
	int n = evt.GetIndex();
	
	MainWindow* wnd = (MainWindow*)mRoot;
	std::string mapName = wnd->GetMapName();
	nEngine::Map* map = nEngine::Resources::inst().require<nEngine::Map> (mapName);
	wnd->SetField(&map->getFieldType(evt.GetItem().GetId()));
}
