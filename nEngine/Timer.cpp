/**
	@file Timer.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Timer.hpp"

namespace nEngine {
	template<> Timer* Timer::Singleton<Timer>::mInstance = NULL;
	// ------------------------------------------------------------------
	Timer::Timer()
		:mNumFrames(0),
		 mLastFrameTime(0.0f)
	{
		if (!QueryPerformanceFrequency((LARGE_INTEGER*) &freq)) {
			throw Error("Timer", "Performance timer not available!");
		} else {
			QueryPerformanceCounter ((LARGE_INTEGER*) &timerStart);
			hasPerfTimer = true;
			resolution = (float) (((double)1.0f) / ((double)freq));
			timerElapsed = timerStart;
		}
	}
	
	// ------------------------------------------------------------------
	Timer::~Timer()
	{

	}
	
	// ------------------------------------------------------------------
	float Timer::getTime ()
	{
		int64 time;
		QueryPerformanceCounter((LARGE_INTEGER *) &time);  

		return ((float)(time - timerStart) * resolution * 1000.0f);
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Timer)

	luaEndMeta()
	
	// ------------------------------------------------------------------
	luaBeginMethods(Timer)

	luaEndMethods()	
	
	// ------------------------------------------------------------------
	bool luaRegisterTimer(lua_State* L)
	{
		luaClass(L, Timer);
		return true;
	}
};
