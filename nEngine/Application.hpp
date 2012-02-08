/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "types.hpp"

namespace nEngine {
    class Application {
    public:
		
		NAPI Application();
        NAPI virtual ~Application();

        NAPI void start();

		NAPI void exit();

		NAPI virtual void onKeyUp (int keyCode, char charCode);
		NAPI std::vector<std::pair<int, int> > getDisplayModes();

		NAPI static void killSingletons();

    protected:

        NAPI void initOpenGL();
		NAPI void initWindow();

		NAPI virtual void onSceneInit();
		NAPI virtual void onSceneDestroy();
		NAPI virtual void onLuaInit(lua_State* L);
		NAPI virtual void onResize (int width, int height);
        NAPI virtual void onLeftClick (int x, int y);
        NAPI virtual void onRightClick (int x, int y);
        NAPI virtual void onMouseMove (int x, int y);

		NAPI void parseCmdLine();

		NAPI void closeWindow();
		NAPI void drawWaitScreen();

        NAPI void draw();
		NAPI void handleMessage(MSG message);
		
		int			mWidth;
		int			mHeight;
		int			mBpp;
		int			mMaxFPS;
		int			mVolume;

		bool		mFullScreen;
		bool		mActive;
		bool		mDone;

		std::string mWindowTitle;
		std::string mInitFile;

		HGLRC		mGlContext;
		HDC			mDeviceContext;
		HWND		mWindowHandle;
		HINSTANCE	mInstance;
    };
};

#endif /*APPLICATION_HPP*/
