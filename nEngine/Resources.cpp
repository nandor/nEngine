/**
	@file Resources.hpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Font.hpp"
#include "Shader.hpp"
#include "Resources.hpp"
#include "JSONObject.hpp"
#include "Console.hpp"
#include "Map.hpp"
#include "ObjectScript.hpp"
using namespace boost::property_tree;

namespace nEngine {
	template<> Resources* Resources::Singleton<Resources>::mInstance = 0;

	// ------------------------------------------------------------------
	Resources::Resources()
	{
		mMemoryUsage = 0;
		mNotUnloadable = 0;
		mGarbageTreshold = 1024 * 1024 * 10;	// 10 Megabytes

		Font::initLibrary();
	}

	// ------------------------------------------------------------------
	Resources::~Resources()
	{
		Font::closeLibrary();

		mResources.clear();
	}

	// ------------------------------------------------------------------
	bool Resources::addResource(Resource* resource, const std::string& groupName)
	{
		if (resource == NULL) {
			return false;
		}
	
		tResourceIter it = mResources.insert(resource->getID(), resource);
		mResourceGroups[groupName].push_back(it);
		return true;
	}
	
	// ------------------------------------------------------------------
	Resource* Resources::findResource(const std::string& id, ResourceType type)
	{
		boost::iterator_range<tResourceIter> range = mResources.equal_range(id);
	
		for (tResourceIter it = range.begin(); it != range.end(); ++it) {
			if (it->second->getType() == type) {
				return it->second;
			}
		}

		return NULL;
	}
		
	// ------------------------------------------------------------------
	int Resources::getMemoryUsage()
	{
		return mMemoryUsage;
	}
	
	// ------------------------------------------------------------------
	void Resources::loadResourceGroup(const std::string& groupName, const std::string& fileName)
	{
		ConsoleLog("Loading resource group '" + groupName + "'");
		try {
			JSONObject* json = Resources::require<JSONObject>(fileName);
			boost::property_tree::ptree& data = json->getRoot();

			BOOST_FOREACH(ptree::value_type& node, data) {
				ptree& item = node.second;
			
				std::string type(item.get<std::string>("type"));

				if (type == "SHADER") {
					std::string id(item.get<std::string>("id"));
					Shader* s = new Shader(id);

					BOOST_FOREACH(ptree::value_type& shader, item.get_child("shaders")) {
						s->load(shader.second.get_value<std::string>());
					}
				
					s->compile();
					Resources::inst().addResource(s, groupName);

					ConsoleLog("\tshader: " + id);
					continue;
				}

				if (type == "FONT") {
					std::string face (item.get<std::string> ("face"));

					Font::loadFace(face);
					
					BOOST_FOREACH(ptree::value_type& font, item.get_child("fonts")) {
						std::string id(font.second.get<std::string>("id"));

						Font* ft = new Font(id, font.second.get<int>("height"));
						Resources::inst().addResource(ft, groupName);

						ConsoleLog("\tfont: " + id);
					}
					Font::unloadFace();
				}

				if (type == "MAP") {
					std::string id(item.get<std::string> ("id"));

					Map* mp = new Map(id);
					mp->loadData(item.get_child("data"));
					Resources::inst().addResource(mp, groupName);

					ConsoleLog("\tmap: " + id);
				}

				if (type == "OBJECT") {
					std::string id(item.get<std::string> ("id"));

					Resources::inst().addResource(new ObjectScript(id, item.get_child("data")), groupName);

					ConsoleLog("\tobject: " + id);
				}

				if (type == "PARTICLE") {
					std::string id(item.get<std::string> ("id"));

					Resources::inst().addResource(new JSONObject(id, item.get_child("data")), groupName);

					ConsoleLog("\tparticles: " + id);
				}
			}
		} catch (std::exception except) {
			throw Error("Resources", std::string(except.what()));
		}
	}
	
	// ------------------------------------------------------------------
	void Resources::unloadResourceGroup(const std::string& groupName)
	{
		std::vector<tResourceIter>::iterator it;

		for (it = mResourceGroups[groupName].begin(); it != mResourceGroups[groupName].end(); ++it) {
			mMemoryUsage -= (*it)->second->getMemoryUsage();
			mResources.erase(*(it));
		}
	}

	// ------------------------------------------------------------------
	luaNewMethod(Resources, loadGroup)
	{
		std::string groupName(luaL_checkstring(L, 1));
		std::string fileName(luaL_checkstring(L, 2));

		Resources::inst().loadResourceGroup(groupName, fileName);
		return 0;
	}

	// ------------------------------------------------------------------
	luaNewMethod(Resources, unloadGroup)
	{
		std::string groupName(luaL_checkstring(L, 1));

		Resources::inst().unloadResourceGroup(groupName);
		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Resources)
	luaEndMeta()

	luaBeginMethods(Resources)
		luaMethod(Resources, loadGroup)
		luaMethod(Resources, unloadGroup)
	luaEndMethods()

	// ------------------------------------------------------------------
	void Resources::luaRegister(lua_State* L)
	{
		luaClass(L, Resources)
	}
};