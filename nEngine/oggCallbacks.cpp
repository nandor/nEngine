/**
    @file       oggCallbacks.cpp
    @author     Licker Nandor

    This file is part of nEngine
    (c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "oggCallbacks.hpp"

namespace nEngine {
	// ------------------------------------------------------------------
	size_t readFile(void* ptr, size_t size, size_t nmemb, void* dataSource)
	{
		return ((File*)dataSource)->read(ptr, size * nmemb);
	}

	// ------------------------------------------------------------------
	int seekFile(void* dataSource, ogg_int64_t offset, int whence)
	{
		return ((File*)dataSource)->set(offset, (FileOffset)whence);
	}

	// ------------------------------------------------------------------
	int closeFile(void* dataSource)
	{
		return true;
	}

	// ------------------------------------------------------------------
	long tellFile(void* dataSource)
	{
		return ((File*)dataSource)->tell();
	}
};