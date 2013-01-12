/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "File.hpp"
#include "DataSource.hpp"
using namespace boost::property_tree;

namespace nEngine {

	// ------------------------------------------------------------------
	DataSource::DataSource(const std::string& id)
		:Resource(id, RESOURCE_DATA)
	{
		readFromFile(getID());
		readAnimData();
	}

	// ------------------------------------------------------------------
	DataSource::DataSource(const std::string& id, ptree& data)
		:Resource(id, RESOURCE_DATA)
	{
		mData = data;
		readAnimData();
	}

	// ------------------------------------------------------------------
	DataSource::DataSource(const std::string& id, const std::string& fileName)
		:Resource(id, RESOURCE_DATA)
	{
		readFromFile(fileName);
		readAnimData();
	}

	// ------------------------------------------------------------------
	void DataSource::readFromFile(const std::string& fileName)
	{
		File* file = Resources::inst().require<File> (fileName);

		if (!file || !file->getData()) {
			throw Error("ObjectScript", getID(), "Cannot open file '" + fileName + "'");
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
	DataSource::~DataSource()
	{

	}

	// ------------------------------------------------------------------
	void DataSource::readAnimData()
	{
		boost::optional<ptree&> anims = mData.get_child_optional("animations");

		if (!anims.is_initialized()) {
			return;
		}

		BOOST_FOREACH(ptree::value_type& v, anims.get()) {
			Animation& anim = mAnimations[v.first];
			anim.setDuration(v.second.get<int>("duration"));
			anim.setFrames(v.second.get<int>("frames"));
			anim.setImage(v.second.get<std::string>("image"));
			anim.setID(v.first);
		}
	}


	// ------------------------------------------------------------------
	Animation& DataSource::getAnimation(const std::string& name)
	{
		return mAnimations[name];
	}

	// ------------------------------------------------------------------
	boost::property_tree::ptree& DataSource::getChild(const std::string& id)
	{
		try {

			return mData.get_child(id);

		} catch (boost::property_tree::ptree_error except) {
			throw Error("JSONObject", getID(), "[json]" + std::string(except.what()));
		}
	}

	// ------------------------------------------------------------------
	Vec2 DataSource::getVec(const std::string& id)
	{		
		try {

			ptree p = mData.get_child(id);
			return Vec2(p.get<float>("x"), p.get<float>("y"));

		} catch (boost::property_tree::ptree_error except) {
			throw Error("JSONObject", getID(), "[json '" + id + "']" + std::string(except.what()));
		}
	}

	// ------------------------------------------------------------------
	Color DataSource::getColor(const std::string& id)
	{
		try {

			ptree p = mData.get_child(id);
			return Color(p.get("r", 0.0f), p.get("g", 0.0f), p.get("b", 0.0f), p.get("a", 1.0f));

		} catch (boost::property_tree::ptree_error except) {
			throw Error("JSONObject", getID(), "[json '" + id + "']" + std::string(except.what()));
		}

	}
};