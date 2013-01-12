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
#include "DataSource.hpp"
#include "Console.hpp"
#include "Map.hpp"
using namespace boost::property_tree;

namespace nEngine {
	template<> Resources* Resources::Singleton<Resources>::__inst = NULL;

	// ------------------------------------------------------------------
	Resources::Resources()
	{
		mMemoryUsage = 0;
		Font::initLibrary();
	}

	// ------------------------------------------------------------------
	Resources::~Resources()
	{
		mResources.clear();
		mResourceGroups.clear();

		Font::closeLibrary();
	}

	// ------------------------------------------------------------------
	Resources& Resources::addResource(Resource* resource, const std::string& groupName)
	{
		if (resource == NULL || resource->getID().length() == 0) {
			return *this;
		}

		if (resource->getType() == RESOURCE_MAP) {
			ResourceType y = resource->getType();
		}

		std::pair<std::string, ResourceType> id(resource->getID(), resource->getType());

		ResourceGroup* mGroup = getGroup(groupName);

		if (mResources.find(id) != mResources.end()) {
			throw Error ("Resources", "Resource already exists: '" + resource->getID() + "'");
		}

		mResources.insert(id, resource);
		getGroup(groupName)->addResource(resource);

		return *this;
	}
	
	// ------------------------------------------------------------------
	Resource* Resources::findResource(const std::string& id, ResourceType type)
	{
		tResourceIter it = mResources.find(std::make_pair(id, type));

		if (it == mResources.end()) {
			return NULL;
		} 

		return it->second;
	}
		
	// ------------------------------------------------------------------
	int Resources::getMemoryUsage()
	{
		return mMemoryUsage;
	}
	
	// ------------------------------------------------------------------
	Resources& Resources::loadGroup(const std::string& package)
	{
		try {
			std::string packageLocation = "zip://" + File::getRawName(package);

			DataSource* json = require<DataSource>(packageLocation + "/header");
			std::string packageName = json->getValue<std::string> ("packageName");
			File::setenv(packageName, packageLocation);
			

			// lua
			boost::optional<ptree&> luas = json->getRoot().get_child_optional("lua");
			if (luas.is_initialized()) {
				BOOST_FOREACH(ptree::value_type& node, luas.get()) {
					luaReadFile(node.second.get_value<std::string>());
				}
			}

			// we may have a single child
			std::string luaFile = json->getRoot().get("lua", "");
			if (luaFile != "") {
				luaReadFile(luaFile);
			}


			// shaders
			boost::optional<ptree&> shaders = json->getRoot().get_child_optional("shader");			
			if (shaders.is_initialized()) {
				BOOST_FOREACH(ptree::value_type& node, shaders.get()) {
					ptree& item = node.second;

					Shader* s = new Shader(node.first);

					BOOST_FOREACH(ptree::value_type& shader, item.get_child("source")) {
						s->load(shader.second.get_value<std::string>());
					}
				
					s->compile();
					Resources::inst().addResource(s, packageName);
				}
			}

			// fonts
			boost::optional<ptree&> fonts = json->getRoot().get_child_optional("font");
			if (fonts.is_initialized()) {
				BOOST_FOREACH(ptree::value_type& node, fonts.get()) {
					ptree& item = node.second;

					std::string face (item.get<std::string> ("face"));

					Font::loadFace(face);
					
					BOOST_FOREACH(ptree::value_type& font, item.get_child("fonts")) {
						std::string id(font.second.get<std::string>("id"));

						Font* ft = new Font(id, font.second.get<int>("height"));
						Resources::inst().addResource(ft, packageName);
					}

					Font::unloadFace();
				}
			}

			// object
			boost::optional<ptree&> data = json->getRoot().get_child_optional("data");
			if (data.is_initialized()) {
				BOOST_FOREACH(ptree::value_type& node, data.get()) {
					Resources::inst().addResource(new DataSource(node.first, node.second.get_value<std::string>()), packageName);
				}
			}

			// map
			boost::optional<ptree&> maps = json->getRoot().get_child_optional("map");
			if (maps.is_initialized()) {
				BOOST_FOREACH(ptree::value_type& node, maps.get()) {
					Map* mp = new Map(node.first);
					mp->loadData(node.second);
					Resources::inst().addResource(mp, packageName);
				}
			}
		} catch (std::exception except) {
			throw Error("Resources", std::string(except.what()));
		}

		return *this;
	}
	
	// ------------------------------------------------------------------
	std::vector<std::string> Resources::getResourceGroupNames()
	{
		std::vector<std::string> res;

		boost::ptr_map<std::string, ResourceGroup*>::iterator it;
		for (it = mResourceGroups.begin(); it != mResourceGroups.end(); ++it) {
			if (it->first != "tmp") {
				res.push_back(it->first);
			}
		}

		return res;
	}
	
	// ------------------------------------------------------------------
	Resources& Resources::clearResources()
	{
		mResourceGroups.clear();
		mResources.clear();
		return *this;
	}
	
	// ------------------------------------------------------------------
	ResourceGroup* Resources::getGroup(const std::string& name)
	{
		boost::ptr_map<std::string, ResourceGroup>::iterator it = mResourceGroups.find(name);

		if (it == mResourceGroups.end()) {
			ResourceGroup* grp = new ResourceGroup(name);
			mResourceGroups.insert(grp->getName(), grp);
			return grp;
		}

		return it->second;
	}
	
	// ------------------------------------------------------------------
	ResourceGroup* Resources::createGroup(const std::string& name)
	{
		
		boost::ptr_map<std::string, ResourceGroup>::iterator it = mResourceGroups.find(name);

		if (it != mResourceGroups.end()) {
			throw Error ("Resources", "Group already exists: '" + name + "'");
		}
		
		ResourceGroup* grp = new ResourceGroup(name);
		mResourceGroups.insert(grp->getName(), grp);
		return grp;
	}

	// ------------------------------------------------------------------
	Resources& Resources::unloadGroup(const std::string& name)
	{
		boost::ptr_map<std::string, ResourceGroup>::iterator grpIter = mResourceGroups.find(name);
		std::vector<tResId> resources = grpIter->second->getResources();

		for (std::vector<tResId>::iterator it = resources.begin(); it != resources.end(); ++it) {
			tResourceIter resIter = mResources.find(*it);
			
			if (resIter != mResources.end()) {
				mResources.erase(resIter);
			}
		}

		mResourceGroups.erase(grpIter);
		return *this;
	}
	
	// ------------------------------------------------------------------
	luaDeclareMethod(Resources, loadPackage)
	{
		std::string groupName(luaL_checkstring(L, 1));
		Resources::inst().loadGroup(groupName);
		return 0;
	}

	// ------------------------------------------------------------------
	luaDeclareMethod(Resources, unloadGroup)
	{
		std::string groupName(luaL_checkstring(L, 1));

		Resources::inst().unloadGroup(groupName);
		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Resources)
	luaEndMeta()

	luaBeginMethods(Resources)
		luaMethod(Resources, loadPackage)
		luaMethod(Resources, unloadGroup)
	luaEndMethods()

	// ------------------------------------------------------------------
	bool luaRegisterResources(lua_State* L)
	{
		luaClass(L, Resources);
		return true;
	}
};