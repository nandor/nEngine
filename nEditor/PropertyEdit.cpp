/**
	@file PropertyEdit.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nEditor.hpp"
#include "nEngine/Shader.hpp"
#include "nEngine/Resources.hpp"

#include "PropertyEdit.hpp"

// ------------------------------------------------------------------
PropertyEdit::PropertyEdit(wxWindow* parent, wxSize& size, wxWindow* root)
	:wxPanel(parent, wxID_ANY, wxDefaultPosition, size)
{
	mRoot = root;
	int toolbarHeight = 30;
	
	mGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxSize(size.GetWidth(), size.GetHeight() - toolbarHeight), 
								wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
}


// ------------------------------------------------------------------
PropertyEdit::~PropertyEdit()
{

}



// ------------------------------------------------------------------
void PropertyEdit::setResourceJSON(const std::string& id)
{
	mGrid->Clear();
}

// ------------------------------------------------------------------
void PropertyEdit::setResourceShader(const std::string& id)
{
	// Set up the property grid
	mGrid->Clear();

    mGrid->Append(new wxPropertyCategory(wxT("Attributes")));
    mGrid->Append(new wxStringProperty(wxT("ID"), wxT("id"), id));
	
	wxArrayString shaderList;
	nEngine::Shader* s = nEngine::Resources::inst().get<nEngine::Shader> (id);
	std::vector<std::string> shaders = s->getShaderNames();
	for (std::vector<std::string>::iterator it = shaders.begin(); it != shaders.end(); ++it) {
		shaderList.Add(*it);
	}

    wxPGProperty* shaderCont = mGrid->Append(new wxArrayStringProperty(wxT("Shaders"), wxT("shaders"), shaderList));
}

// ------------------------------------------------------------------
void PropertyEdit::setResourceMap(const std::string& id)
{
	mGrid->Clear();
}
