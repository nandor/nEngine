/**
	@file TileList.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef TILELIST_HPP
#define TILELIST_HPP

#include <wx/wx.h>
#include <wx/mstream.h>
#include <wx/listctrl.h>

class TileList : public wxListCtrl {
public:
	TileList(wxWindow* parent, wxSize size, wxWindow* root);
	~TileList();

	void Refresh();

private:

	DECLARE_EVENT_TABLE();
	void OnColResize(wxListEvent& evt);
	void OnItemSelect(wxListEvent& evt);

private:

	wxWindow* mRoot;
};

#endif /*TILELIST_HPP*/