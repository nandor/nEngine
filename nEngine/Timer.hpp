/**
	@file Timer.hpp
	@author Licker Nandor

	Only time will tell...

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef TIMER_HPP
#define TIMER_HPP

#include "types.hpp"
#include "Singleton.hpp"
#include "Error.hpp"
#include "Lua.hpp"

namespace nEngine {
	class Timer : public Singleton<Timer> {
	public:
		NAPI Timer();
		NAPI ~Timer();
		
        NAPI float getTime ();

		NAPI float getFrameTime()
		{
			return getTime() - mLastFrameTime;
		}

		NAPI float getFPS() 
		{
			return 1000.0f * mNumFrames / (getTime() - mLastFrameTime);
		}

		NAPI void frameEnd()
		{
			if (mLastFrameTime >= 10000.0f) {
				mLastFrameTime = getTime();
				mNumFrames = 0;
			}

			++mNumFrames;
		}

	private:
		
		float resolution;               
		bool hasPerfTimer; 
		int64 freq;  
		int64 timerStart;
		int64 timerElapsed;

		int64 mNumFrames;

		float mLastFrameTime;
		float mFPS;
    };

	bool luaRegisterTimer(lua_State* L);
};

#endif /*TIMER_HPP*/
