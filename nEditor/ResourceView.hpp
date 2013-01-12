/**
	@file ResourceView.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef RESOURCEVIEW_HPP
#define RESOURCEVIEW_HPP


#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include <wx/wx.h>

class ResourceView : public wxTreeCtrl {
public:
	ResourceView(wxWindow* parent, wxSize& size, wxWindow* root);
	~ResourceView();

	void Refresh();

private:

	void CreateIcons();

private:

	DECLARE_EVENT_TABLE();

	void OnItemSelect(wxTreeEvent& evt);
	void OnItemMenu(wxTreeEvent& evt);
	void OnItemActivate(wxTreeEvent& evt);

private:
	int mIconSize;

	wxWindow* mRoot;
	wxTreeItemId mRootID;
};

#endif /*RESOURCEVIEW_HPP*/