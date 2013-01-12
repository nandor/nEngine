/**
	@file MapEditor.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include "nEngine/Camera.hpp"
#include "nEngine/Map.hpp"

#include <gl/glew.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>

class MapEditor : public wxGLCanvas {
public:
	MapEditor(wxWindow* parent, wxSize& size, wxWindow* root);
	~MapEditor();
	
	void Save();
	void OnPaint(wxPaintEvent& evt);
	void InitOpenGL();
	void SetOrtho();
	void SetMap(const std::string& id);
	void SetField(nEngine::FieldType* field);
	void PlaceTile(wxPoint& mouse);

private:

	DECLARE_EVENT_TABLE();

	void OnLeftDown(wxMouseEvent& evt);
	void OnLeftUp(wxMouseEvent& evt);
	
	void OnRightDown(wxMouseEvent& evt);
	void OnRightUp(wxMouseEvent& evt);

	void OnMouseLeave(wxMouseEvent& evt);
	void OnMouseMoved(wxMouseEvent& evt);

	void OnBackgroundPaint(wxEraseEvent& evt);

private:

	nEngine::Camera* mCamera;
	wxWindow* mRoot;

	nEngine::FieldType* mField;
	wxPoint mInitialOffset;
	wxPoint mInitialMouse;

	/// Is the screen being dragged?
	bool mDragging;

	/// Is the user placing objects?
	bool mPlacing;
};

#endif /*MAPEDITOR_HPP*/