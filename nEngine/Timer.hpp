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
		/**
			Constructor
		*/
		NAPI Timer();

		/**
			Destructor
		*/
		NAPI ~Timer();
		
		/**
			Return the time passed since startup
			@return		seconds,milliseconds
		*/
        NAPI float getTime ();

		/**
			Get the time passed while rendering this frame
			@return duration
		*/
		NAPI float getFrameTime()
		{
			return getTime() - mLastFrameTime;
		}

		/**
			Get the fps
			@param	fps
		*/
		NAPI float getFPS() 
		{
			return 1000.0f * mNumFrames / (getTime() - mLastFrameTime);
		}

		/**
			Finish a frame
		*/
		NAPI void Timer::frameEnd();

		/**
			Queue a call to a function
			@param func			Boost function object
			@param time			Delay
		*/		
		NAPI void queueAction(boost::function<void(float)> func, float time);

		/**
			Queue a call to a lua function
			@param luaFuncRef	Reference to a lua Function
			@param time			Delay
		*/
		NAPI void queueAction(int luaFuncRef, float time);

	private:

		class TimerObject {
		public:

			float mExpireTime;
			boost::function<void(float)> mFunc;
			bool mIsLua;
			int mLuaRef;

		public:

			bool operator < (const TimerObject& a) const {
				return this->mExpireTime < a.mExpireTime;
			}

			void operator () (float t) const;
		};

		typedef std::set<TimerObject>::iterator tQueueIter;

		std::set<TimerObject> mQueue;

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
