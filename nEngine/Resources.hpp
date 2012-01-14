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
				Resource* res = findResource(id, T::sType);

				if (res != NULL) {
					return (T*)res;
				}

				res = new T(id);
				
				mMemoryUsage += res->getMemoryUsage();
				
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
				Resource* res = findResource(id, T::sType);

				if (res != NULL) {
					return (T*)res;
				}
				
				return NULL;
            }
			
            /**
                Add a new resource and set its id
                @param resource     Resource to add
                @param id           Group name
            */
            NAPI Resources& addResource(Resource* resource, const std::string& groupName = "tmp");

			/**
				Load a resource group from a file
				@param groupName	Name of the resource group
				@param fileName		Resource group descriptor file
			*/
			NAPI Resources& loadResourceGroup(const std::string& groupName, const std::string& fileName);

			/**
				Unload a resource group
				@param groupName	Name of the resource group
			*/
			NAPI Resources& unloadResourceGroup(const std::string& groupName);
			
			/**
				Get the memory used by the resources
				@return				Used memory in bytes
			*/
			NAPI int getMemoryUsage();

			/**
				Get the names of resource groups
				@return				std::vector
			*/
			NAPI std::vector<std::string> getResourceGroupNames();

			/**
				Return all resources in a group
				@param name			Group Name
			*/
			NAPI std::vector<std::pair<std::string, ResourceType> > Resources::getResourcesInGroup(const std::string& name);

        private:
			
			/**
				Get a resource by its id and type
				@param id			ID of the resource	
				@param type			The type 

				@return				Pointer to the resource
			*/
			NAPI Resource* findResource(const std::string& id, ResourceType type);
			
        private:
			/// Memory usage
			int mMemoryUsage;

			/// resource id
			typedef std::pair<std::string, ResourceType> tResId;

            /// iterator for the resources
            typedef boost::ptr_map<tResId, Resource>::iterator tResourceIter;
			
            /// map contatining resource data
            boost::ptr_map<tResId, Resource> mResources;
			
			/// map for resource groups
			std::map<std::string, std::vector<tResId> > mResourceGroups;
    };

    /**
        Register the lua callbacks used by this module
        @param L            Lua state
    */
    NAPI bool luaRegisterResources (lua_State* L);
};

#endif /*RESOURCES_HPP*/

