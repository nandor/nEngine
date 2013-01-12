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
#include "Map.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "SceneNode.hpp"
#include "Singleton.hpp"

namespace nEngine {
	enum SceneRenderMode {
		SCENE_DRAW_OBJECTS = 1,
		SCENE_DRAW_MARKERS = 2
	};

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
		NAPI Scene& setCamera(const std::string& camera);

		/**
			Set the camera to be used
			@param camera		Pointer to the camera
		*/
		NAPI Scene& setCamera(Camera* cam);

		/**
			Set a new map to be used
			@param map			Pointer to the map
		*/
		NAPI Scene& setMap(Map* map);
		
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
		NAPI Scene& addNode(SceneNode* obj);

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
		NAPI Scene& destroyScene();

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
			Set the way the renderer draws the scene
			@param drawMode				OR-d SceneRenderModes
		*/
		NAPI Scene& setDrawMode(int drawMode);

		/**
			Add a new light to the scene
			@param l					Pointer to the light
		*/
		NAPI Scene& addLight(Light* l);

		/**
			Remove a light from the scene
			@param id					Name of the id
		*/
		NAPI Scene& removeLight(const std::string& id);
		
		/**
			Return the name of the selected object
			@param pos					Position of the object
			@return						Pointer to the object
		*/
		NAPI SceneNode* selectNode(Vec2& pos);

		/**
			Remove a node from the scene
			@param id					ID of the scene node
		*/
		NAPI void removeNode(const std::string& id);

	public:

		/// Number of available lights
		static const int sNumLights = 8;

		/// Max number of selection hits
		static const int sNumHits = 64;

	private:

		/**
			Check if a tile is on the screen
			@param tile					Position
			@return						bool			
		*/
		bool onScreen(Vec2& tile);

		/**
			Apply all the lights
		*/
		void setLights();

	private:
		
		/// Size of tiles
		Vec2 mTileSize;

		/// Main camera
		Camera* mCamera;

		/// Pointer to the map
		Map* mMap;

		/// Map containig the objects
		std::map<std::string, SceneNode*> mNodes;
		
		/// Iterator for the map containing the objects
		typedef std::map<std::string, SceneNode*>::iterator tNodeIter;
			
		/// Rendering mode
		unsigned mDrawMode;
		
		/// Selected node
		SceneNode* mSelected;

		/// map containing lights
		std::map<std::string, Light*> mLights;
		
		/// Iterator for the map containing lights
		typedef std::map<std::string, Light*>::iterator tLightIter;
	};

	/**
		Register the lua objects associated with the scene
		@param L			Lua state
	*/
    NAPI bool luaRegisterScene (lua_State* L);
};

#endif /*SCENE_HPP*/