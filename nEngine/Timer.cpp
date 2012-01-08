/**
	@file Timer.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Timer.hpp"
#include "Console.hpp"

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
	void Timer::frameEnd()
	{
		if (mLastFrameTime >= 10000.0f) {
			mLastFrameTime = getTime();
			mNumFrames = 0;
		}

		++mNumFrames;
		
		float time = getTime();

		std::vector<tQueueIter> toErase;

		tQueueIter seqEnd = mQueue.end();
		for (tQueueIter it = mQueue.begin(); it != mQueue.end(); ++it) {
			if (it->mExpireTime < time) {
				(*it) (time);
				toErase.push_back(it);
			}
		}

		for(std::vector<tQueueIter>::iterator it = toErase.begin(); it != toErase.end(); ++it) {
			mQueue.erase(*it);
		}
	}
	
	// ------------------------------------------------------------------
	void Timer::queueAction(boost::function<void(float)> func, float time)
	{
		TimerObject obj;
		obj.mExpireTime = getTime() + time;
		obj.mFunc = func;
		obj.mIsLua = false;
		mQueue.insert(obj);
	}
	
	// ------------------------------------------------------------------
	void Timer::queueAction(int luaFuncRef, float time)
	{
		TimerObject obj;
		obj.mExpireTime = getTime() + time;
		obj.mLuaRef = luaFuncRef;
		obj.mIsLua = true;
		mQueue.insert(obj);
	}
	
	// ------------------------------------------------------------------
	void Timer::TimerObject::operator() (float time) const
	{
		if (mIsLua) {			
			if (mLuaRef == LUA_NOREF) {
				throw Error("Tile", "Invalid reference!");
			}

			lua_State* L = luaGlobalState();
		
			lua_rawgeti(L, LUA_REGISTRYINDEX, mLuaRef);
			lua_pushnumber(L, time);
			lua_pcall(L, 1, 0, 0);	
		} else {
			mFunc (time);
		}
	}

	// ------------------------------------------------------------------
	luaNewMethod(Timer, getTime)
	{
		lua_pushnumber(L, Timer::inst().getTime());
		return 1;
	}

	// ------------------------------------------------------------------
	luaNewMethod(Timer, queue)
	{
		if (lua_type(L, 1) != LUA_TFUNCTION) {
			throw Error("Timer", "Object needs to be a function");
		}

		lua_pushvalue(L, 1);
		Timer::inst().queueAction(luaL_ref(L, LUA_REGISTRYINDEX), luaL_checknumber(L, 2));

		return 0;
	}

	// ------------------------------------------------------------------
	luaBeginMeta(Timer)

	luaEndMeta()
	
	// ------------------------------------------------------------------
	luaBeginMethods(Timer)
		luaMethod(Timer, getTime)
		luaMethod(Timer, queue)
	luaEndMethods()	
	
	// ------------------------------------------------------------------
	bool luaRegisterTimer(lua_State* L)
	{
		luaClass(L, Timer);
		return true;
	}
};
