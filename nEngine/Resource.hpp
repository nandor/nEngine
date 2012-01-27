/**
	@file Resource.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "nHeaders.hpp"
#include "types.hpp"

namespace nEngine {
    enum ResourceType {
        RESOURCE_NONE,
        RESOURCE_FILE,
        RESOURCE_IMAGE,
        RESOURCE_FONT,
        RESOURCE_SHADER,
        RESOURCE_CHAT,
		RESOURCE_MAP,
        RESOURCE_ARCHIVE,
		RESOURCE_OBJECT,
		RESOURCE_NPC,
		RESOURCE_JSON,
		RESOURCE_SOUND,

		RESOURCE_NUM
    };

    class Resource {
    public:
		/**
			Construct a new resource
			@param id			ID of the resource
			@param type			Type of the resource
		*/
        Resource(const std::string& id, ResourceType type);
        
		/**
			Destroy the resource
		*/
		virtual ~Resource();

		/**
			Return the id of the resource
			@return				Id
		*/
        std::string getID () 
		{
            return mId;
        }

		/**
			Return the type of the resource
			@return				Type
		*/
        ResourceType getType () 
		{
            return mType;
        }

		/**
			Return the ammount of used memory
			@return				Memory in bytes
		*/
		virtual int getMemoryUsage() 
		{
			return sizeof(*this);
		}

		/**
			Return the name of the resource group
			@return				Group name
		*/
		std::string getGroupName()
		{
			return mGroupName;
		}

		/**
			Set the name of the resource group
			@param groupName	Name of the group
		*/
		void setGroupName(std::string groupName)
		{
			mGroupName = groupName;
		}

    private:

		/// ID of the resource
		std::string         mId;

		/// Type of the resource
        ResourceType        mType;

		/// Time last used
		float				mTimeUsed;

		/// The group of this resource
		std::string			mGroupName;
    };
};

#endif /* RESOURCE_HPP */
