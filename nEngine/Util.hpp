/**
	@file Util.hpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef UTIL_HPP
#define UTIL_HPP

#include "types.hpp"
#include "Color.hpp"
#include "Vec2.hpp"
#include "Font.hpp"
#include "Shader.hpp"
#include "Image.hpp"

namespace nEngine 
{
    NAPI void glPrint(Font* ft, int x, int y, const std::string& fmt, ...);

	NAPI void glPrint(Font* ft, int x, int y, unsigned length, const std::string& fmt);

	NAPI Vec2 getScreenSize();

	/**
		Return a Win32 error message

		@return			Staring containing the error message
	*/
	NAPI std::string getWin32Error();

	/**
		Convert a hexadecimal character to an integer
	*/
	NAPI int charToInt(char c);
};

#endif /* UTIL_HPP */
