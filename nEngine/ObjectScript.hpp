/**
    @file       ObjectScript.hpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef OBJECTSCRIPT_HPP
#define OBJECTSCRIPT_HPP

#include "types.hpp"
#include "Resource.hpp"
#include "File.hpp"
#include "Animation.hpp"

namespace nEngine {
	class ObjectScript : public Resource {
	public:
		/**
			Constructor
		*/
		ObjectScript(const std::string& id);
		
		/**
			Build the script with an id and a json node
		*/
		ObjectScript(const std::string& id, boost::property_tree::ptree& tree);

		/**
			Destructor
		*/
		~ObjectScript();
		
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
				throw Error("ObjectScript", getID(), "[json]" + std::string(except.what()));
			}
		}

		/**
			Get a child node from the json config
			@param id			Name of the node
			@return				The child node
		*/
		boost::property_tree::ptree getChild(const std::string& id);
		
		Animation* getAnimation(const std::string& id);
	public:
		static const ResourceType sType = RESOURCE_OBJECT;

	private:

		/// Map to hold the animations
		std::map<std::string, Animation> mAnimations;

		/// Iterator for the map with animations
		typedef std::map<std::string, Animation>::iterator tAnimIter;

		/// Property tree (for JSON)
		boost::property_tree::ptree mData;
	};
};

#endif /*OBJECTSCRIPT_HPP*/