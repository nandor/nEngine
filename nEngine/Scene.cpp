/**
	@file Resources.hpp
	@author Licker Nandor

    @brief Scene Graph for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Scene.hpp"

namespace nEngine {
	template<> Scene* Scene::Singleton<Scene>::mInstance = NULL;

	// ------------------------------------------------------------------
	Scene::Scene()
		:mTileSize(120, 60),
		 mActive(false)
	{
		mCamera = NULL;
		mMap = NULL;
	}
	
	
	// ------------------------------------------------------------------
	Scene::~Scene()
	{
		for (tNodeIter it = mNodes.begin(); it != mNodes.end(); ++it) {
			delete it->second;
		}
	}
	

	// ------------------------------------------------------------------
	void Scene::draw()
	{	
		if (mActive) {
			glPushMatrix();

			if (mCamera) {
				mCamera->focus();
			} else {
				glLoadIdentity();
			}
		
			if (mMap) {				
				mMap->draw();
			}

			for (tNodeIter it = mNodes.begin(); it != mNodes.end(); ++it) {
				if (mMap->isVisible(it->second->getTile())) {
					it->second->draw();
				}
			}

			glPopMatrix();
		}
	}
	

	// ------------------------------------------------------------------
	void Scene::update()
	{
		if (mActive) {
			Map* map = getMap();

			map->shadow();

			for (tNodeIter it = mNodes.begin(); it != mNodes.end(); ++it) {
				if (it->second) {
					it->second->update();

					if (it->second->isHighlighted()) {
						map->highlight(it->second->getTile(), it->second->getHighlightRange());
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------------
	void Scene::addNode(SceneNode* c)
	{
		tNodeIter it = mNodes.find(c->getID());

		if (it != mNodes.end()) {
			throw Error("Scene", "Node '" + c->getID() + "' already exists!");
		}

		mNodes.insert(make_pair(c->getID(), c));
	}

	
	// ------------------------------------------------------------------
	std::string Scene::getCamera()
	{
		return mCamera->getID();
	}
			
	// ------------------------------------------------------------------
	SceneNode* Scene::getNode(const std::string& name)
	{
		tNodeIter it = mNodes.find(name);

		if (it == mNodes.end()) {
			throw Error ("Scene", "Object does not exist: '" + name + "'");
		}

		return it->second;
	}


	// ------------------------------------------------------------------
	Map* Scene::getMap()
	{
		return mMap;
	}

		
	// ------------------------------------------------------------------
	void Scene::setMap(Map* map)
	{
		if (map == NULL) {
			throw Error("Scene", "Invalid map");
		}

		map->callLuaMethod("onSceneInit");

		mMap = map;
	}


	// ------------------------------------------------------------------
	void Scene::setCamera(const std::string& cameraName)
	{
		tNodeIter it = mNodes.find(cameraName);

		if (it == mNodes.end()) {
			throw Error("Scene", "Camera not found: " + cameraName);
		}

		mCamera = dynamic_cast<Camera*>(it->second);
	}
	

	// ------------------------------------------------------------------
	Vec2 Scene::getTileAt(int x, int y) 
	{
		if (mMap == NULL || mCamera == NULL) {
			return Vec2();
		}
		
		Vec2 absPos = Vec2(x, y) + mCamera->getOffset();
		
		return Vec2 (
			(int)((absPos.getX() / 2 - absPos.getY()) / mTileSize.getY()),
			(int)((absPos.getX() / 2 + absPos.getY()) / mTileSize.getY())
		);
	}
	
	// ------------------------------------------------------------------
	Vec2 Scene::getCameraOffset()
	{
		if (!mCamera) {
			return Vec2(0, 0);
		}

		return mCamera->getOffset();
	}
	
	// ------------------------------------------------------------------
	void Scene::destroyScene()
	{

	}

	// ------------------------------------------------------------------
	SceneNode* Scene::getNodeAt(Vec2 tile)
	{
		for (tNodeIter it = mNodes.begin(); it != mNodes.end(); ++it) {
			if (it->second->getTile() == tile) {
				return it->second;
			}
		}

		return NULL;
	}
	

	// ------------------------------------------------------------------
	luaNewMethod(Scene, add)
	{
		SceneNode* node = *(SceneNode**)luaGetInstance(L, 1, "SceneNode");
		Scene::inst().addNode(node);
		return 0;
	}

	// ------------------------------------------------------------------
	luaNewMethod(Scene, setCamera)
	{
		Scene::inst().setCamera(std::string(luaL_checkstring(L, 1)));
		return 0;
	}

	// ------------------------------------------------------------------
	luaNewMethod(Scene, get)
	{
		SceneNode* node = Scene::inst().getNode(std::string(luaL_checkstring(L, 1)));
		luaInstance(L, SceneNode, node);
		return 1;
	}

	
	// ------------------------------------------------------------------
	luaNewMethod(Scene, start)
	{
		Scene::inst().start();
		return 0;
	}
	
	// ------------------------------------------------------------------
	luaNewMethod(Scene, stop)
	{
		Scene::inst().stop();
		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Scene)
	luaEndMeta()
	
	
	// ------------------------------------------------------------------
	luaBeginMethods(Scene)
		luaMethod(Scene, add)
		luaMethod(Scene, get)
		luaMethod(Scene, start)
		luaMethod(Scene, stop)
		luaMethod(Scene, setCamera)
	luaEndMethods()
	

	// ------------------------------------------------------------------
	void Scene::luaRegister(lua_State* L)
	{
		luaClass(L, Scene)
	}
};