/**
	@file types.h
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef TYPES_H
#define TYPES_H

#include "nHeaders.hpp"

#define INF 0x3f3f3f3f
#define NENGINE_WIN32

#ifdef NENGINE_EXPORTS
#define NAPI __declspec(dllexport)
#else
#define NAPI __declspec(dllimport)
#endif

namespace nEngine {
	typedef signed char int8;
	typedef signed short int16;
	typedef signed int int32;
	typedef signed long long int64;

	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;

	namespace Key {
		enum KeyCodes {
			Left	= VK_LEFT,
			Right	= VK_RIGHT,
			Up		= VK_UP,
			Down	= VK_DOWN
		};
	};
};

#endif /*TYPES_H*/
