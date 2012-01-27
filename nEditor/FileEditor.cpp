/**
	@file FileEditor.cpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "FileEditor.hpp"

std::regex shaderRegex(".+.(frag|vert)$");
std::regex luaRegex(".+.lua$");

BEGIN_EVENT_TABLE(FileEditor, wxScintilla)
	EVT_SCI_MODIFIED(wxID_ANY, FileEditor::OnModified)
END_EVENT_TABLE()

// ------------------------------------------------------------------
FileEditor::FileEditor(wxWindow* parent, nEngine::File* file)
	:wxScintilla(parent, wxID_ANY)
{
	mFile = file;
	this->SetLabel(mFile->getID());
	this->SetText(std::string((char*)mFile->getData()));
	this->StyleSetVisible(wxSCI_STYLE_LINENUMBER, true);

	mEditable = nEngine::Resources::inst().getGroup(file->getGroupName())->isEditable();

	if (!mEditable) {
		this->SetReadOnly(true);
	}
	
	wxColour commentColour(0, 100, 0);
	wxColour stringColor(100, 0, 0);
	wxColour varColour(0, 0, 200);

	if (std::regex_match(file->getID(), shaderRegex)) {
		this->SetLexer(wxSCI_LEX_CPP);

		this->StyleSetForeground(wxSCI_C_COMMENT, commentColour);
		this->StyleSetForeground(wxSCI_C_COMMENTLINE, commentColour);
		this->StyleSetForeground(wxSCI_C_COMMENTDOC, commentColour);
		
		this->StyleSetForeground(wxSCI_C_STRING, stringColor);
		
		this->StyleSetForeground(wxSCI_C_NUMBER, wxColour(0, 0, 100));
	} else {
		this->SetLexer(wxSCI_LEX_LUA);

		this->StyleSetForeground(wxSCI_LUA_COMMENT, commentColour);
		this->StyleSetForeground(wxSCI_LUA_COMMENTLINE, commentColour);
		this->StyleSetForeground(wxSCI_LUA_COMMENTDOC, commentColour);
		
		this->StyleSetForeground(wxSCI_LUA_STRING, stringColor);

		this->StyleSetForeground(wxSCI_C_NUMBER, wxColour(0, 0, 100));
	}

	mModified = false;
}


// ------------------------------------------------------------------
FileEditor::~FileEditor()
{

}

// ------------------------------------------------------------------
void FileEditor::OnModified(wxScintillaEvent& evt)
{
	mModified = true;
}

// ------------------------------------------------------------------
void FileEditor::Save()
{
	mFile->setData((nEngine::uint8*)this->GetText().c_str(), this->GetText().length());
	mFile->write();
	mModified = false;
}

// ------------------------------------------------------------------
std::string FileEditor::GetFileName()
{
	return mFile->getID();
}