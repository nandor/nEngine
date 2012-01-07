/**
	@file Resources.hpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include "types.hpp"
#include "Lua.hpp"
#include "Singleton.hpp"
#include "Resource.hpp"
#include "Timer.hpp"

namespace nEngine {
    /**
        @brief Loads and manages resources

        Resource manager for nEngine. Manages the resources
        and provides quick access to them. It can load
        resources from files or from zip archives and can
        package game assets into archives.
    */
    class Resources : public Singleton<Resources> {
        public:
			NAPI Resources ();
            /**
                Destructor
            */
            NAPI ~Resources ();

            /**
                Get a list of the resources which have a certain type
                @param type         Type to find

                @return             List of resources
            */
            NAPI std::vector<Resource*> getResourceByType(ResourceType type);

            /**
                Check if a resource exists
                @param id           ID of the resource to find

                @return             True if resource exists, false otherwise
            */
            NAPI bool hasResource(const std::string& id);

            /**
                Get a resouce by id. If it does not exist, it will be created. If it is of a wrong
				type, it returns null

                @class T            Type of the resource
                @param id           ID of the resource to find

                @return             Pointer to the resource or NULL if it does not exist
            */
            template<class T> T* require(const std::string& id)
            {
				float time = Timer::inst().getTime();
				Resource* res = findResource(id, T::sType);

				if (res != NULL) {
					res->setTimeUsed(time);
					return (T*)res;
				}

				res = new T(id);
				
				res->setTimeUsed(time);
				mMemoryUsage += res->getMemoryUsage();

				if (!res->isUnloadable()) {
					mNotUnloadable += res->getMemoryUsage();
					mGarbageTreshold = min(mGarbageTreshold, mNotUnloadable);
				}

				addResource(res, "tmp");
				return (T*)res;
            };

            /**
                Get a font
                @param id           ID of the font

                @return             Pointer to the resource or none if it does not exist
            */
            template<class T> T* get(const std::string& id)
            {
				float time = Timer::inst().getTime();
				Resource* res = findResource(id, T::sType);

				if (res != NULL) {
					res->setTimeUsed(time);
					return (T*)res;
				}
				
				return NULL;
            }
			
            /**
                Add a new resource and set its id
                @param resource     Resource to add
                @param id           Group name
            */
            NAPI bool addResource(Resource* resource, const std::string& groupName = "tmp");

			/**
				Load a resource group from a file
				@param groupName	Name of the resource group
				@param fileName		Resource group descriptor file
			*/
			NAPI void loadResourceGroup(const std::string& groupName, const std::string& fileName);

			/**
				Unload a resource group
				@param groupName	Name of the resource group
			*/
			NAPI void unloadResourceGroup(const std::string& groupName);
			
            /**
                Register the lua callbacks used by this module
                @param L            Lua state
            */
            NAPI static void luaRegister (lua_State* L);


			/**
				Get the memory used by the resources
				@return				Used memory in bytes
			*/
			NAPI int getMemoryUsage();

			/**
				Free a resource
				@param id			ID of the resource
				@param type			Type of the resource
			*/
			NAPI void free(const std::string& id, ResourceType type);

        private:
			
			/**
				Get a resource by its id and type
				@param id			ID of the resource	
				@param type			The type 

				@return				Pointer to the resource
			*/
			NAPI Resource* findResource(const std::string& id, ResourceType type);
			
        private:
			/// Max ammount of used garbage
			int mGarbageTreshold;

			/// Memory usage
			int mMemoryUsage;

			/// Ammount of memory that can't be unloaded
			int mNotUnloadable;

            /// iterator for the resources
            typedef boost::ptr_multimap<std::string, Resource>::iterator tResourceIter;
			
            /// map contatining resource data
            boost::ptr_multimap<std::string, Resource> mResources;
			
			/// map for resource groups
			std::map<std::string, std::vector<tResourceIter>> mResourceGroups;
    };
};

#endif /*RESOURCES_HPP*/

