/**
	@file Resources.hpp
	@author Licker Nandor

    @brief Scene Graph for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef SCENE_HPP
#define SCENE_HPP

#include "types.hpp"
#include "Error.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "SceneNode.hpp"
#include "Singleton.hpp"

namespace nEngine {
	/**
		Scene manager
	*/
	class Scene : public Singleton<Scene> {
	public:
		/**
			Create a new scene
		*/
		NAPI Scene();

		/**
			Free all the scene nodes
		*/
		NAPI ~Scene();

		/**
			Update the scene
		*/
		NAPI void update();

		/**
			Draw the entire scene
		*/
		NAPI void draw();

		/**
			Return the name of the camera
			@return				Name of the camera
		*/
		NAPI std::string getCamera ();
		
		/**
			Return the map
			@return				Pointer to the map
		*/
		NAPI Map* getMap();
		
		/**
			Set the name of the camera to be used
			@param camera		Name of the camera
		*/
		NAPI void setCamera(const std::string& camera);

		/**
			Set a new map to be used
			@param map			Pointer to the map
		*/
		NAPI void setMap(Map* map);
		
		/**
			Get a pointer to an object
			@param id			Id of the object
			@return				Pointer to the object
		*/
		NAPI SceneNode* getNode(const std::string& id);

		/**
			Add a new objec to the scene manager
			@param obj			Pointer to the object
		*/
		NAPI void addNode(SceneNode* obj);

		/**
			Register the lua objects associated with the scene
			@param L			Lua state
		*/
        NAPI static void luaRegister (lua_State* L);

		/**
			Get the coordinate of a tile at a certain position
			@param x			mouse x
			@param y			mouse y
			@return				pair containing coordinates
		*/
		NAPI Vec2 getTileAt(int x, int y);

		/**
			Destroy the scene
		*/
		NAPI void destroyScene();

		/**
			Check if an object is on a certain tile
			@param tile			Tile
		*/
		NAPI SceneNode* getNodeAt(Vec2 tile);

		/**
			Get the camera offset
			@return				offset
		*/
		NAPI Vec2 getCameraOffset();

		/**
			Activate the scene
		*/
		NAPI void stop() {
			mActive = false;
		}

		/**
			Deactivate the scene
		*/
		NAPI void start() {
			mActive = true;
		}

	private:
		/// Size of tiles
		Vec2 mTileSize;

		/// Main camera
		Camera* mCamera;

		/// Pointer to the map
		Map* mMap;

		/// Scene activation flag
		bool mActive;

		/// Map containig the objects
		std::map<std::string, SceneNode*> mNodes;
		
		/// Iterator for the map containing the objects
		typedef std::map<std::string, SceneNode*>::iterator tNodeIter;
	};
};

#endif /*SCENE_HPP*/