/**
    @file       oggCallbacks.hpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#ifndef OGGCALLBACKS_HPP
#define OGGCALLBACKS_HPP

#include "File.hpp"

namespace nEngine {
	/**
		fread equivalent for a nEngine file
		@param ptr				Pointer to the buffer
		@param size				Size of an element
		@param nmemb			Number of elements
		@param dataSource		Pointer to the nEngine file
		@return					Number of bytes read
	*/
	size_t readFile(void* ptr, size_t size, size_t nmemb, void* dataSource);

	/**
		Move the file pointer
		@param dataSource		Pointer to the file
		@param offset			Where
		@param whence			From where
	*/
	int seekFile(void* dataSource, ogg_int64_t offset, int whence);

	/**
		Close a nEngine file (do nothing actually :D)
		@param dataSource		Pointer to the file
		@return	true
	*/
	int closeFile(void* dataSource);
	
	/**
		Tell how many bytes were read from a file
		@param dataSource		Pointer to the file
		@return	Position
	*/
	long tellFile(void* dataSource);

	static ov_callbacks oggReader = {
		readFile,
		seekFile,
		closeFile,
		tellFile
	};
};

#endif /*OGGCALLBACKS_HPP*/