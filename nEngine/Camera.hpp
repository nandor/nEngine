/**
	@file Camera.hpp
	@author Licker Nandor
	
	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "nHeaders.hpp"
#include "types.hpp"
#include "Util.hpp"
#include "Object.hpp"

namespace nEngine {
	/**
		Handle a camera
	*/
	class Camera : public SceneNode {
	public:
		Camera (const std::string& camera);
		~Camera ();
	
		/**
			Activate this camera
		*/
		void focus();

		/**
			Draw the camera
		*/
		void draw();

		/**
			Update the camera
		*/
		void update();

		/**
			Start following an object
			@param id			ID of the object to follow
		*/
		void follow (const std::string& id);

		/**
			Start following an object
			@param obj			Pointer to the object to follow
		*/
		void follow (SceneNode* obj);

		/**
			Set the position of the camera
			@param pos			Position
		*/
		void setPosition(const Vec2& p);
		
		/**
			Get the offset of the camera
		*/
		Vec2 getOffset();

	private:
		
		/// Position to be centered on
		Vec2 mCenter;

		/// Object to be followed
		SceneNode* mToFollow;
	};

	bool luaRegisterCamera(lua_State* L);
};

#endif /*CAMERA_HPP*/