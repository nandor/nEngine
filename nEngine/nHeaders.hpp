
#ifndef NHEADERS_HPP
#define NHEADERS_HPP

// STD Includes
#include <cmath>
#include <cstdio>
#include <sstream>

// STL Includes
#include <set>
#include <map>
#include <list>
#include <queue>
#include <vector>
#include <string>
#include <utility>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <exception>


#include <boost/bind.hpp>
#include <boost/foreach.hpp> 
#include <boost/function.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/json_parser.hpp>


// C++ includes
#include <regex>

// OpenGL Includes
#include <GL/glew.h>

// Sound
#include <al/al.h>
#include <al/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

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
	#undef min
	#undef max
#endif

#ifdef _MSC_VER
	#pragma warning(disable: 4800)
	#pragma warning (disable: 4244)
#endif 

#endif /*NHEADERS_HPP*/