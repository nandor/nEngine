/**
	Precompiled header for nEngine

	@file stdafx.h
*/

#ifndef STDAFX_H
#define STDAFX_H

// STD Includes
#include <cmath>
#include <cstdio>
#include <sstream>
#include <iostream>

// STL Includes
#include <map>
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <utility>
#include <iterator>
#include <algorithm>

// Pointer container
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>

// Boost Includes
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp> 
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/json_parser.hpp>

// OpenGL Includes
#include <GL/glew.h>

// C Includes
extern "C" {
	#include <minizip/unzip.h>
    #include <lua/lua.h>
    #include <lua/lauxlib.h>
    #include <lua/lualib.h>
}

// Library includes
#include <png.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN   
	#include <windows.h>
#endif

#ifdef _MSC_VER
	#pragma warning(disable: 4800)
	#pragma warning (disable: 4244)
#endif 


#endif /*STDAFX_H*/