/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "File.hpp"
#include "JSONObject.hpp"
using namespace boost::property_tree;


namespace nEngine {

	// ------------------------------------------------------------------
	JSONObject::JSONObject(const std::string& id)
		:Resource(id, RESOURCE_JSON)
	{
		File* file = Resources::inst().require<File> (id);

		if (!file || !file->getData()) {
			throw Error("ObjectScript", getID(), "Cannot open file '" + id + "'");
		}

		try {		
			// Initialise the property tree
			std::stringstream ss((char*)file->getData());
			read_json(ss, mData);

		} catch (std::exception except) {
			throw Error("ObjectScript", getID(), std::string(except.what()));
		}
	}
	// ------------------------------------------------------------------
	JSONObject::JSONObject(const std::string& id, ptree& data)
		:Resource(id, RESOURCE_JSON)
	{
		mData = data;
	}

	// ------------------------------------------------------------------
	JSONObject::~JSONObject()
	{

	}

	// ------------------------------------------------------------------
	boost::property_tree::ptree& JSONObject::getChild(const std::string& id)
	{
		try {

			return mData.get_child(id);

		} catch (boost::property_tree::ptree_error except) {
			throw Error("JSONObject", getID(), "[json]" + std::string(except.what()));
		}
	}

	// ------------------------------------------------------------------
	Vec2 JSONObject::getVec(const std::string& id)
	{		
		try {

			ptree p = mData.get_child(id);
			return Vec2(p.get<float>("x"), p.get<float>("y"));

		} catch (boost::property_tree::ptree_error except) {
			throw Error("JSONObject", getID(), "[json '" + id + "']" + std::string(except.what()));
		}
	}

	// ------------------------------------------------------------------
	Color JSONObject::getColor(const std::string& id)
	{
		try {

			ptree p = mData.get_child(id);
			return Color(p.get("r", 0.0f), p.get("g", 0.0f), p.get("b", 0.0f), p.get("a", 1.0f));

		} catch (boost::property_tree::ptree_error except) {
			throw Error("JSONObject", getID(), "[json '" + id + "']" + std::string(except.what()));
		}

	}
};