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
        std::string getID () {
            return mId;
        }

		/**
			Return the type of the resource
			@return				Type
		*/
        ResourceType getType () {
            return mType;
        }

        void setType (ResourceType _type) {
            mType = _type;
        }

        void setID (const std::string& _id) {
            mId = _id;
        }
		
		virtual int getMemoryUsage() 
		{
			return sizeof(*this);
		}

		void setUnloadable(bool unloadable)
		{
			mIsUnloadable = unloadable;
		}

		bool isUnloadable()
		{
			return mIsUnloadable;
		}

    private:
		/// ID of the resource
		std::string         mId;

		/// Type of the resource
        ResourceType        mType;

		/// Time last used
		float				mTimeUsed;

		/// Is the resource unloadable?
		bool				mIsUnloadable;
    };
};

#endif /* RESOURCE_HPP */
