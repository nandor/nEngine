/**
	@file ResourceGroup.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef RESOURCEGROUP_HPP
#define RESOURCEGROUP_HPP

#include "Resource.hpp"

namespace nEngine {
	class ResourceGroup {
	public:

		/**
			Create a new resource group
		*/
		NAPI ResourceGroup(const std::string& name);

		/**
			Destroy the resource group
		*/
		NAPI ~ResourceGroup();

		/**
			Check if the resource group is editable
			@return				Is the group editable?
		*/
		NAPI bool isEditable()
		{
			return mEditable;
		}

		/**
			Set the editable property
			@param edit			True / False
		*/
		NAPI void setEditable(bool edit)
		{
			mEditable = edit;
		}

		/**
			Get the name of the group
			@return					Group name
		*/
		std::string getName()
		{
			return mName;
		}

		/**
			Return the ammount of memory used by a resource
		*/
		NAPI int getMemoryUsage();
						
		/**
			Add a resource
			@param res				Pointer to the resource
		*/
		NAPI ResourceGroup* addResource(Resource* res);

		/**
			Get all the resources in the group
			@return					A vector
		*/
		NAPI std::vector<std::pair<std::string, ResourceType> > getResources();
	private:

		/// Name of the resource group
		std::string mName;

		/// List of resources
		std::vector<Resource*> mResources;

		/// Check if the group is editable
		bool mEditable;

		/// If the resource group is unloadable
		bool mUnloadable;
	};
};

#endif /*RESOURCEGROUP_HPP*/