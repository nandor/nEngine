/**
	@file ResourceView.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "MapEditor.hpp"
#include "MainWindow.hpp"

#include "nEngine/nHeaders.hpp"
#include "nEngine/Resources.hpp"
#include "nEngine/Scene.hpp"

#include "Icons/camera.xpm"
#include "Icons/object.xpm"

BEGIN_EVENT_TABLE(MapEditor, wxGLCanvas)
	EVT_RIGHT_DOWN(MapEditor::OnRightDown)
	EVT_RIGHT_UP(MapEditor::OnRightUp)

	EVT_LEFT_DOWN(MapEditor::OnLeftDown)
	EVT_LEFT_UP(MapEditor::OnLeftUp)

	EVT_MOTION(MapEditor::OnMouseMoved)
	EVT_LEAVE_WINDOW(MapEditor::OnMouseLeave)

	EVT_PAINT(MapEditor::OnPaint)
    EVT_ERASE_BACKGROUND(MapEditor::OnEraseBackground)
END_EVENT_TABLE()

// ------------------------------------------------------------------
MapEditor::MapEditor(wxWindow* parent, wxSize& size, wxWindow* root)
	:wxGLCanvas(parent, wxID_ANY, wxDefaultPosition, size),
	 mDragging(false),
	 mPlacing(false),
	 mField(NULL)
{
	mRoot = root;
	this->SetLabel("mapEditor");
	this->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

// ------------------------------------------------------------------
MapEditor::~MapEditor()
{

}


// ------------------------------------------------------------------
void MapEditor::OnBackgroundPaint(wxEraseEvent& evt)
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
	MainWindow* wnd = (MainWindow*)mRoot;
	nEngine::Map* map = nEngine::Resources::inst().get<nEngine::Map> (wnd->GetMapName());
	
	if (map) {
		map->setShadows(false);

		nEngine::Scene::inst()
			.setMap(map)
			.start();
	
		this->Refresh();
		this->Update();
	}
}


// ------------------------------------------------------------------
void MapEditor::OnLeftDown(wxMouseEvent& evt)
{
	mPlacing = true;
	this->SetCursor(wxCursor(wxCURSOR_HAND));
}

// ------------------------------------------------------------------
void MapEditor::OnLeftUp(wxMouseEvent& evt)
{
	mPlacing = false;
	PlaceTile(evt.GetPosition());
	this->SetCursor(wxCursor(wxCURSOR_ARROW));
}


// ------------------------------------------------------------------
void MapEditor::OnRightDown(wxMouseEvent& evt)
{
	mDragging = true;
	mInitialOffset = wxPoint(mCamera->getPosition().getX(), mCamera->getPosition().getY());
	mInitialMouse = evt.GetPosition();
	this->SetCursor(wxCursor(wxCURSOR_SIZING));
}

// ------------------------------------------------------------------
void MapEditor::OnRightUp(wxMouseEvent& evt)
{
	mDragging = false;
	this->SetCursor(wxCursor(wxCURSOR_ARROW));
}

// ------------------------------------------------------------------
void MapEditor::OnMouseMoved(wxMouseEvent& evt)
{
	wxPoint mouse = evt.GetPosition();

	if (mDragging) {
		wxPoint pt = mInitialOffset + mInitialMouse - evt.GetPosition();
		mCamera->setPosition(nEngine::Vec2(pt.x, pt.y));

		this->Refresh();
		return;
	}

	if (mPlacing && mField != NULL) {
		PlaceTile(mouse);
		return;
	}
}


// ------------------------------------------------------------------
void MapEditor::PlaceTile(wxPoint& mouse)
{
	MainWindow* wnd = (MainWindow*)mRoot;
	nEngine::Vec2 v = nEngine::Scene::inst().getTileAt(mouse.x, mouse.y);
	nEngine::Map* map = nEngine::Resources::inst().get<nEngine::Map> (wnd->GetMapName());
		
	if (map && map->hasTile(v)) {
		map->setTileID(v, mField->mID);
		this->Refresh();
	}

}

// ------------------------------------------------------------------
void MapEditor::OnMouseLeave(wxMouseEvent& evt)
{
	this->SetCursor(wxCursor(wxCURSOR_ARROW));
	mDragging = mPlacing = false;
}



// ------------------------------------------------------------------
void MapEditor::SetField(nEngine::FieldType* fld)
{
	mField = fld;
}


// ------------------------------------------------------------------
void MapEditor::Save()
{
	MainWindow* wnd = (MainWindow*)mRoot;
	nEngine::Map* map = nEngine::Resources::inst().get<nEngine::Map> (wnd->GetMapName());
	if (map != NULL) {
		map->writeMapData();
	}
}
