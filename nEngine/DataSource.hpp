/**
	@file DataSource.hpp
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
#include "Animation.hpp"

namespace nEngine {
	class DataSource : public Resource {
	public:
		DataSource(const std::string& id);
		DataSource(const std::string& id, const std::string& fileName);
		DataSource(const std::string& id, boost::property_tree::ptree& data);

		~DataSource();
		
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
		
		/**
			Get an animation data
			@param id			ID of the animation
		*/		
		Animation& getAnimation(const std::string& id);

	private:
		
		/**
			Read the JSON / XML data from a file
			Throws an error if a problem is encountered
			@param name			Name of the file
		*/
		void readFromFile(const std::string& name);

		/**
			Read animation data from the file
		*/
		void readAnimData();

	public:
		
		static const ResourceType sType = RESOURCE_DATA;
	
	private:
		/// Property tree
		boost::property_tree::ptree mData;

		/// Map to hold the animations
		std::map<std::string, Animation> mAnimations;

		/// Iterator for the map with animations
		typedef std::map<std::string, Animation>::iterator tAnimIter;
	};
};

#endif /*JSONOBJECT_HPP*/