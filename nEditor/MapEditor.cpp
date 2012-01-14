/**
	@file ResourceView.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "MapEditor.hpp"
#include "nEngine/nHeaders.hpp"
#include "nEngine/Resources.hpp"
#include "nEngine/Scene.hpp"

#include "Icons/camera.xpm"
#include "Icons/object.xpm"

BEGIN_EVENT_TABLE(MapEditor, wxGLCanvas)
	EVT_PAINT(MapEditor::OnPaint)
	EVT_LEFT_DOWN(MapEditor::OnLeftDown)
	EVT_LEFT_UP(MapEditor::OnLeftUp)
	EVT_MOTION(MapEditor::OnMouseMoved)
	EVT_LEAVE_WINDOW(MapEditor::OnMouseLeave)
    EVT_ERASE_BACKGROUND(MapEditor::OnEraseBackground)
END_EVENT_TABLE()

// ------------------------------------------------------------------
MapEditor::MapEditor(wxWindow* parent, wxSize& size)
	:wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, size),
	 mMouseDragged(false)
{
	this->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

// ------------------------------------------------------------------
MapEditor::~MapEditor()
{

}

// ------------------------------------------------------------------
void MapEditor::OnPaint(wxPaintEvent& evt)
{
	SetCurrent();
	wxPaintDC(this);
	
	SetOrtho();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	nEngine::Scene::inst().draw();
	glFlush();
	SwapBuffers();
}

void MapEditor::SetOrtho() 
{
	int width = GetSize().x, height = GetSize().y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0, width, height, 0, -10000.0f, 1000000.0f);
	glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
}


// ------------------------------------------------------------------
void MapEditor::InitOpenGL()
{
	SetCurrent();
	glewInit();
	
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Textures
	glEnable(GL_TEXTURE_2D);
	
	// nEngine 
	mCamera = new nEngine::Camera("editCam");
	mCamera->setMarker(false);

	// Load some images
	nEngine::Resources::inst()
		.addResource(new nEngine::Image("cameraMarker", camera_xpm))
		.addResource(new nEngine::Image("objectMarker", object_xpm));

	// Prepare the scene
	nEngine::Scene::inst()
		.addNode(mCamera)
		.setCamera(mCamera)
		.setDrawMode(nEngine::SCENE_DRAW_OBJECTS | nEngine::SCENE_DRAW_MARKERS);
}


// ------------------------------------------------------------------
void MapEditor::SetMap(const std::string& id)
{
	nEngine::Map* map = nEngine::Resources::inst().get<nEngine::Map> ("tutorial");
	map->setShadows(false);

	nEngine::Scene::inst().setMap(map);
	nEngine::Scene::inst().start();
	
	this->Refresh();
	this->Update();
}


// ------------------------------------------------------------------
void MapEditor::OnLeftDown(wxMouseEvent& evt)
{
	mMouseDragged = true;
	mInitialOffset = wxPoint(mCamera->getPosition().getX(), mCamera->getPosition().getY());
	mInitialMouse = evt.GetPosition();
}

// ------------------------------------------------------------------
void MapEditor::OnLeftUp(wxMouseEvent& evt)
{
	mMouseDragged = false;
}

// ------------------------------------------------------------------
void MapEditor::OnMouseLeave(wxMouseEvent& evt)
{
	mMouseDragged = false;
}


// ------------------------------------------------------------------
void MapEditor::OnMouseMoved(wxMouseEvent& evt)
{
	if (mMouseDragged) {
		wxPoint pt = mInitialOffset + mInitialMouse - evt.GetPosition();
		mCamera->setPosition(nEngine::Vec2(pt.x, pt.y));
		this->Refresh();
	}
}


// ------------------------------------------------------------------
void MapEditor::OnBackgroundPaint(wxEraseEvent& evt)
{

}
