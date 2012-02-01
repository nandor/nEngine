/**
	@file JSONObject.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef JSONOBJECT_HPP
#define JSONOBJECT_HPP

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"
#include "Resource.hpp"

namespace nEngine {
	class JSONObject : public Resource {
	public:
		JSONObject(const std::string& id);
		JSONObject(const std::string& id, boost::property_tree::ptree& data);

		~JSONObject();
		
		/**
			Get a value from the json config
			@param id			Name of the value
			@return				Value of the node
		*/
		template <typename T> T getValue(const std::string& id)
		{
			try {
				return mData.get<T> (id);
			} catch (boost::property_tree::ptree_error except) {
				throw Error("JSONObject", getID(), "[json]" + std::string(except.what()));
			}
		}

		/**
			Get a child node from the json config
			@param id			Name of the node
			@return				The child node
		*/
		boost::property_tree::ptree& getChild(const std::string& id);

		/**
			Get the root of the json tree
			@param id			Name of the node
			@return				The child node
		*/
		boost::property_tree::ptree& getRoot()
		{
			return mData;
		}
		
		/**
			Return a Vec2
			@param id			ID of the value
		*/
		Vec2 getVec(const std::string& id);

		/**
			Get a Color
			@param id			ID of the Color
		*/
		Color getColor(const std::string& Color);

	public:
		
		static const ResourceType sType = RESOURCE_JSON;
	
	private:
		/// Property tree
		boost::property_tree::ptree mData;
	};
};

#endif /*JSONOBJECT_HPP*/