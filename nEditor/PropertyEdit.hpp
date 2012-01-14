/**
	@file ProepertyEdit.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef PROPERTYEDIT_HPP
#define PROPERTYEDIT_HPP

#include <wx/wx.h>
#include <wx/propgrid/propgrid.h>

class PropertyEdit : public wxPanel {
public:
	PropertyEdit(wxWindow* parent, wxSize& size, wxWindow* root);
	~PropertyEdit();


	void setResourceJSON(const std::string& id);
	void setResourceShader(const std::string& id);

private:
	
	wxWindow* mRoot;
	wxPropertyGrid* mGrid;
	wxPanel* mToolbar;
};

#endif /*PROPERTYEDIT_HPP*/