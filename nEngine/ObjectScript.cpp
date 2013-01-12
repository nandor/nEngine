/**
    @file       ObjectScript.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "ObjectScript.hpp"
using namespace boost::property_tree;

namespace nEngine {
	
	
	// ------------------------------------------------------------------
	ObjectScript::ObjectScript(const std::string& id)
		:Resource(id, RESOURCE_OBJECT)
	{
		File* file = Resources::inst().require<File> (id);

		if (!file || !file->getData()) {
			throw Error("ObjectScript", getID(), "Cannot open file '" + id + "'");
		}
		
		try {		
			// Initialise the property tree
			std::stringstream ss((char*)file->getData());
			read_json(ss, mData);


			BOOST_FOREACH(ptree::value_type& v, mData.get_child("animations")) {
				Animation anim;

				anim.setDuration(v.second.get<int> ("duration"));
				anim.setFrames(v.second.get<int> ("frames"));
				anim.setImage(v.second.get<std::string> ("image"));
				anim.setID(v.first);

				mAnimations.insert(make_pair(v.first, anim));
			}

		} catch (std::exception except) {
			throw Error("ObjectScript", getID(), "[json]" + std::string(except.what()));
		}
	}

	// ------------------------------------------------------------------
	ObjectScript::ObjectScript(const std::string& id, ptree& tree)
		:Resource(id, RESOURCE_OBJECT)
	{
		
		try {	
			mData = tree;

			BOOST_FOREACH(ptree::value_type& v, mData.get_child("animations")) {
				Animation anim;

				anim.setDuration(v.second.get<int> ("duration"));
				anim.setFrames(v.second.get<int> ("frames"));
				anim.setImage(v.second.get<std::string> ("image"));
				anim.setID(v.first);

				mAnimations.insert(make_pair(v.first, anim));
			}
		} catch (std::exception except) {
			throw Error("ObjectScript", getID(), "[json]" + std::string(except.what()));
		}
	}

	// ------------------------------------------------------------------
	ObjectScript::~ObjectScript()
	{

	}

	// ------------------------------------------------------------------
	boost::property_tree::ptree ObjectScript::getChild(const std::string& id)
	{
		try {
			return mData.get_child(id);
		} catch (boost::property_tree::ptree_error except) {
			throw Error("ObjectScript", getID(), "[json]" + std::string(except.what()));
		}
	}


	// ------------------------------------------------------------------
	Animation* ObjectScript::getAnimation(const std::string& id)
	{
		tAnimIter it = mAnimations.find(id);

		if (it == mAnimations.end()) {
			throw Error ("ObjectScript", getID(), "Animation not found: '" + id + "'");
		}

		return &it->second;
	}
};
