/**
	@file Resources.hpp
	@author Licker Nandor

    @brief Scene Graph for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Scene.hpp"
#include "Error.hpp"

namespace nEngine {
	template<> Scene* Scene::Singleton<Scene>::__inst = NULL;

	// ------------------------------------------------------------------
	Scene::Scene()
		:mTileSize(120, 60),
		 mDrawMode(SCENE_DRAW_OBJECTS)
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

		for (tLightIter it = mLights.begin(); it != mLights.end(); ++it) {
			delete it->second;
		}
	}
	

	// ------------------------------------------------------------------
	void Scene::draw()
	{	
		glPushMatrix();
		glLoadIdentity();

		if (mCamera) {
			mCamera->focus();
		}

		if (!mMap) {	
			return;
		} else {
			glInitNames();
			
			mMap->draw();
			for (tNodeIter it = mNodes.begin(); it != mNodes.end(); ++it) {
				if (onScreen(it->second->getPosition())) {
					it->second->draw();
				}
			}			
		}

		glPopMatrix();
	}
	
	// ------------------------------------------------------------------
	SceneNode* Scene::selectNode(Vec2& pos)
	{
		if (!mCamera) {
			return NULL;
		}

		pos += mCamera->getOffset();

		
		for (tNodeIter it = mNodes.begin(); it != mNodes.end(); ++it) {
			if (onScreen(it->second->getPosition()) && it->second->intersects(pos)) {
				return it->second;
			}
		}		

		return NULL;
	}

	// ------------------------------------------------------------------
	void Scene::update()
	{
		if (mMap == NULL) {
			return;
		}
	
		for (tNodeIter it = mNodes.begin(); it != mNodes.end(); ++it) {
			if (it->second) {
				it->second->update();
			}
		}
	}
	
	// ------------------------------------------------------------------
	Scene& Scene::addNode(SceneNode* c)
	{
		tNodeIter it = mNodes.find(c->getID());

		if (it != mNodes.end()) {
			throw Error("Scene", "Node '" + c->getID() + "' already exists!");
		}
		
		mNodes.insert(std::make_pair(c->getID(), c));
		return *this;
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
	void Scene::removeNode(const std::string& id)
	{
		tNodeIter it = mNodes.find(id);

		if (it == mNodes.end()) {
			return;
		}

		delete it->second;
		mNodes.erase(it);
	}

	// ------------------------------------------------------------------
	Map* Scene::getMap()
	{
		return mMap;
	}

		
	// ------------------------------------------------------------------
	Scene& Scene::setMap(Map* map)
	{
		if (map == NULL) {
			throw Error("Scene", "Invalid map");
		}
		
		mMap = map;
		map->callLuaMethod("onSceneInit");

		return *this;
	}


	// ------------------------------------------------------------------
	Scene& Scene::setCamera(const std::string& cameraName)
	{
		tNodeIter it = mNodes.find(cameraName);

		if (it == mNodes.end()) {
			throw Error("Scene", "Camera not found: " + cameraName);
		}

		mCamera = dynamic_cast<Camera*>(it->second);
		return *this;
	}
	
	// ------------------------------------------------------------------
	Scene& Scene::setCamera(Camera* cam)
	{
		tNodeIter it = mNodes.find(cam->getID());

		if (it == mNodes.end()) {
			mNodes.insert(std::make_pair(cam->getID(), (SceneNode*)cam));
		}

		mCamera = cam;
		return *this;
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
	Scene& Scene::destroyScene()
	{
		return *this;
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
	bool Scene::onScreen(Vec2& pos)
	{
		Vec2 pt = Scene::inst().getMap()->getTileSize();
		Vec2 screenSize = getScreenSize();
		
		Vec2 off = pos - Scene::inst().getCameraOffset();		

		return -pt < off && off < screenSize + pt;
	}
		
	// ------------------------------------------------------------------
	Scene& Scene::setDrawMode(int drawMode)
	{
		mDrawMode = drawMode;
		return *this;
	}
		
	// ------------------------------------------------------------------
	Scene& Scene::addLight(Light* l)
	{
		if (l == NULL) {
			throw Error ("Scene", "addLight", "Invalid argument!");
		}
		
		tLightIter it = mLights.find(l->getID());

		if (it != mLights.end()) {
			throw Error("Scene", "addLight", "Light already exists!");
		}

		mLights.insert(std::make_pair(l->getID(), l));
		setLights();

		if (mMap != NULL) {
			mMap->buildLightMap(mLights);
		}

		return *this;
	}

	// ------------------------------------------------------------------
	void Scene::setLights()
	{
		for (unsigned i = 0; i < sNumLights; ++i) {
			glDisable(GL_LIGHT0 + i);
		}

		int numLights = 0;
		for (tLightIter it = mLights.begin(); it != mLights.end(); ++it) {
			it->second->glUse(numLights++);
		}

		if (numLights > sNumLights) {
			throw Error ("Scene", "Too many light sources!");
		}
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Scene, add)
	{
		SceneNode* node = *(SceneNode**)luaGetInstance(L, 1, "SceneNode");
		Scene::inst().addNode(node);
		return 0;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(Scene, addLight)
	{
		Light* l = *(Light**)luaGetInstance(L, 1, "Light");
		Scene::inst().addLight(l);
		return 0;
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Scene, setCamera)
	{
		Scene::inst().setCamera(std::string(luaL_checkstring(L, 1)));
		return 0;
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Scene, get)
	{
		SceneNode* node = Scene::inst().getNode(std::string(luaL_checkstring(L, 1)));
		luaInstance(L, SceneNode, node);
		return 1;
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Scene, setMap)
	{
		if (lua_isstring(L, 1)) {
			std::string mapName(luaL_checkstring(L, 1));
			Map* map = Resources::inst().get<Map> (mapName);

			if (map == NULL) {
				return 0;
			}

			Scene::inst().setMap(map);
		}
		return 0;
	}


	// ------------------------------------------------------------------
	luaBeginMeta(Scene)
	luaEndMeta()
	
	// ------------------------------------------------------------------
	luaBeginMethods(Scene)
		luaMethod(Scene, add)
		luaMethod(Scene, addLight)
		luaMethod(Scene, get)
		luaMethod(Scene, setCamera)
		luaMethod(Scene, setMap)
	luaEndMethods()
	

	// ------------------------------------------------------------------
	bool luaRegisterScene(lua_State* L)
	{
		luaClass(L, Scene);
		return true;
	}
};