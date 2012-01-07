/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "types.hpp"
#include "Lua.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include "Util.hpp"
#include "Scene.hpp"
#include "Timer.hpp"
#include "Chat.hpp"
#include "Console.hpp"
#include "GUI.hpp"
#include "Object.hpp"
#include "NPC.hpp"
#include "ChatBox.hpp"
#include "Particles.hpp"

namespace nEngine {
    class Application {
    public:
		
		NAPI Application();
        NAPI ~Application();

        NAPI void start();

		NAPI void exit();

		NAPI virtual void onSceneInit();
		NAPI virtual void onSceneDestroy();

		NAPI virtual void onResize (int width, int height);

        NAPI virtual void onLeftClick (int x, int y);
        NAPI virtual void onRightClick (int x, int y);
        NAPI virtual void onMouseMove (int x, int y);

		NAPI virtual void onKeyUp (int keyCode, char charCode);

    private:

        NAPI void initOpenGL();
		NAPI void initWindow();

        NAPI void draw();
		NAPI void handleMessage(MSG message);
		
		int			mWidth;
		int			mHeight;
		int			mBpp;
		int			mMaxFPS;

		bool		mFullScreen;
		bool		mActive;
		bool		mDone;

		std::string mWindowTitle;

		HGLRC		mGlContext;
		HDC			mDeviceContext;
		HWND		mWindowHandle;
		HINSTANCE	mInstance;
    };
};

#endif /*APPLICATION_HPP*/
