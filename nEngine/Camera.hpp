/**
	@file Camera.hpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "types.hpp"
#include "nHeaders.hpp"
#include "Util.hpp"
#include "Object.hpp"

namespace nEngine {
	/**
		Handle a camera
	*/
	class Camera : public SceneNode {
	public:
		NAPI Camera (const std::string& camera);
		NAPI ~Camera ();
	
		/**
			Activate this camera
		*/
		NAPI void focus();

		/**
			Draw the camera
		*/
		NAPI void draw();
		
		/**
			Draw a marker
		*/
		NAPI void drawMarker();

		/**
			Update the camera
		*/
		NAPI void update();

		/**
			Start following an object
			@param id			ID of the object to follow
		*/
		NAPI void follow (const std::string& id);

		/**
			Start following an object
			@param obj			Pointer to the object to follow
		*/
		NAPI void follow (SceneNode* obj);

		/**
			Set the position of the camera
			@param pos			Position
		*/
		NAPI void setPosition(const Vec2& p);
		
		/**
			Get the position of the camera
			@return				Position
		*/
		NAPI Vec2 getPosition();

		/**
			Get the offset of the camera
		*/
		NAPI Vec2 getOffset();

	private:
		
		/// Position to be centered on
		Vec2 mCenter;

		/// Object to be followed
		SceneNode* mToFollow;
	};

	NAPI bool luaRegisterCamera(lua_State* L);
};

#endif /*CAMERA_HPP*/