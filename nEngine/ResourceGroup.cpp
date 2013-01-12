/**
	@file File.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "ResourceGroup.hpp"
#include "Resources.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	ResourceGroup::ResourceGroup(const std::string& name)
		:mName(name),
		 mEditable(false)
	{

	}


	// ------------------------------------------------------------------
	ResourceGroup::~ResourceGroup()
	{

	}

	// ------------------------------------------------------------------
	std::vector<std::pair<std::string, ResourceType> > ResourceGroup::getResources()
	{
		std::vector<std::pair<std::string, ResourceType> > resources;

		for (std::vector<Resource*>::iterator it = mResources.begin(); it != mResources.end(); ++it) {
			resources.push_back(std::make_pair((*it)->getID(), (*it)->getType()));
		}
	
		return resources;
	}

	// ------------------------------------------------------------------
	ResourceGroup* ResourceGroup::addResource(Resource* res)
	{
		res->setGroupName(mName);
		mResources.push_back(res);
		return this;
	}
}