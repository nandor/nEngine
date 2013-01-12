/**
	@file Resource.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Resource.hpp"

namespace nEngine {
	Resource::Resource(const std::string& id, ResourceType type)
		:mId(id),
		 mType(type),
		 mEditable(true)
	{

	}

	Resource::~Resource()
	{
	
	}
};