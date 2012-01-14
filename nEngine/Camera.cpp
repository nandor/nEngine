/**
	@file Camera.cpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Camera.hpp"
#include "Scene.hpp"

namespace nEngine {	
	// ------------------------------------------------------------------
	Camera::Camera(const std::string& id)
		:mCenter(0, 0),
		 mToFollow(NULL),
		 SceneNode(id, "Camera")
	{
		
	}

	
	// ------------------------------------------------------------------
	Camera::~Camera()
	{

	}

	// ------------------------------------------------------------------
	void Camera::draw()
	{

	}
	
	// ------------------------------------------------------------------
	void Camera::update()
	{

	}
	
	// ------------------------------------------------------------------
	void Camera::follow(SceneNode* obj)
	{
		mToFollow = obj;
	}

	
	// ------------------------------------------------------------------
	void Camera::focus()
	{
		Vec2 point = -this->getOffset();

		glLoadIdentity();
		glTranslatef(point.getX(), point.getY(), 0.0f);
	}


	// ------------------------------------------------------------------
	Vec2 Camera::getOffset()
	{
		Vec2 center = mCenter;
		if (mToFollow != NULL) {
			center = mToFollow->getPosition(); 
		}
				
		return center - getScreenSize() / 2;
	}
	
	// ------------------------------------------------------------------
	Vec2 Camera::getPosition()
	{
		if (mToFollow != NULL) {
			return mToFollow->getPosition(); 
		}

		return mCenter;
	}

	// ------------------------------------------------------------------
	void Camera::setPosition(const Vec2& pos)
	{
		mCenter = pos;
	}

	// ------------------------------------------------------------------
	void Camera::drawMarker()
	{

	}
	
	// ------------------------------------------------------------------
	luaNewMethod(Camera, new)
	{
		Camera* c = new Camera(luaL_checkstring(L, 1));
		luaInstance(L, Camera, c);
		return 1;
	}
	
	// ------------------------------------------------------------------
	luaNewMethod(Camera, follow)
	{
		Camera* c = *(Camera**)luaGetInstance(L, 1, "Camera");
		std::string name(luaL_checkstring(L, 2));
		c->follow(Scene::inst().getNode(name));
		return 0;
	}
	
	// ------------------------------------------------------------------
	luaBeginMeta(Camera)
		luaMethod(Camera, follow)
	luaEndMeta()

	luaBeginMethods(Camera)
		luaMethod(Camera, new)
	luaEndMethods()
	
	// ------------------------------------------------------------------
	bool luaRegisterCamera(lua_State* L)
	{
		luaInheritClass(L, Camera, SceneNode)
		return true;
	}
};