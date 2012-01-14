/**
	@file MapEditor.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP

#include "nEngine/Camera.hpp"

#include <gl/glew.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>

class MapEditor : public wxGLCanvas {
public:
	MapEditor(wxWindow* parent, wxSize& size);
	~MapEditor();

	void OnPaint(wxPaintEvent& evt);
	void InitOpenGL();
	void SetOrtho();
	void SetMap(const std::string& id);

private:

	DECLARE_EVENT_TABLE();

	void OnLeftDown(wxMouseEvent& evt);
	void OnLeftUp(wxMouseEvent& evt);
	void OnMouseLeave(wxMouseEvent& evt);
	void OnMouseMoved(wxMouseEvent& evt);
	void OnBackgroundPaint(wxEraseEvent& evt);
private:
	nEngine::Camera* mCamera;

	wxPoint mInitialOffset, mInitialMouse;
	bool mMouseDragged;
};

#endif /*MAPEDITOR_HPP*/