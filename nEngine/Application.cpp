/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Application.hpp"
#include "Console.hpp"
#include "ChatBox.hpp"
#include "GUI.hpp"
#include "SoundManager.hpp"

#define NENGINE_DRAW_WAIT (WM_USER + 0x0001)
#define NENGINE_RELOAD (WM_USER + 0x0002)

namespace nEngine {
	namespace po = boost::program_options;

	// ------------------------------------------------------------------
	LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
	{		
		if (msg == WM_CLOSE) {			
			PostMessage(handle, WM_QUIT, NULL, NULL);
			return 0;
		}
		if (msg == WM_PAINT) {
			PostMessage(handle, NENGINE_RELOAD, NULL, NULL);
			return 0;
		}
		
		return DefWindowProc(handle, msg, wParam, lParam);
	}

	// ------------------------------------------------------------------
	Application::Application()
		:mInitFile("")
	{
		mGlContext = NULL;
		mDeviceContext = NULL;
		mInstance = NULL;
		mWindowHandle = NULL;
	
		luaRegisterEngine();
		luaReadFile("fs://data/lua/lib.lua");

		try {
			parseCmdLine();
		} catch (std::exception err) {
			throw Error("Application", "Invalid command line: " + std::string(err.what()));
		}

		srand(time(NULL));
	}
	
	// ------------------------------------------------------------------
	Application::~Application()
	{
		closeWindow();

		killSingletons();

		if (luaGlobalState()) {
			closeLua();
		}
	}
	
	
	// ------------------------------------------------------------------
	void Application::parseCmdLine()
	{
		po::options_description desc ("Options");

		desc.add_options()
			("help", "Display command line options")
			("width", po::value<int>(), "Window width")
			("height", po::value<int>(), "Window height")
			("fullscreen", "Start in fullscreen mode")
			("config", po::value<std::string>()->default_value("fs://data/lua/config.lua"), "Configuration script")
			("init", po::value<std::string>()->default_value("fs://data/lua/init.lua"), "Initialisation script");


		po::variables_map vm;	
		po::basic_parsed_options<char> opt = po::parse_command_line(__argc, __argv, desc);
		po::store(opt, vm);

		if (vm.count("help")) {
			std::stringstream ss;
			desc.print(ss);

			MessageBox(NULL, ss.str().c_str(), "Options", MB_ICONINFORMATION);
		}
		
		mWindowTitle = "nEngine";
		mFullScreen = false;
		mBpp = 32;
		mMaxFPS = 40;
		mVolume = 20;

		if (vm.count("config") && vm["config"].as<std::string>() != "none") {			
			luaReadFile(vm["config"].as<std::string>());
		
			mWidth = luaGetGlobalInt("displayWidth");
			mHeight = luaGetGlobalInt("displayHeight");
			mFullScreen = luaGetGlobalBoolean("fullScreen");

			mMaxFPS = luaGetGlobalInt("maxFPS");
		}

		if (vm.count("init") && vm["init"].as<std::string>() != "none") {			
			mInitFile = vm["init"].as<std::string>();
		}
		
		mWidth = vm.count("width") ? vm["width"].as<int>() : mWidth;
		mHeight = vm.count("height") ? vm["height"].as<int>() : mHeight;
		mFullScreen = vm.count("fullscreen") ? true : mFullScreen;
	}

	// ------------------------------------------------------------------
	void Application::killSingletons()
	{
		Resources::kill();
		Timer::kill();
		Scene::kill();
		Console::kill();
		GUI::kill();
		ChatBox::kill();
		SoundManager::kill();
	}

	// ------------------------------------------------------------------
	void Application::onResize(int width, int height)
	{
		mWidth = width, mHeight = height;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, mWidth, mHeight);
		glOrtho(0, mWidth, mHeight, 0, -10000.0f, 1000000.0f);
		glMatrixMode(GL_MODELVIEW);
	}
	
	// ------------------------------------------------------------------
	void Application::initWindow()
	{
		mInstance = GetModuleHandle(NULL);  

		RECT WindowRect;                        
		WindowRect.left = (long)0;                
		WindowRect.right = (long)mWidth;                  
		WindowRect.top = (long)0;                     
		WindowRect.bottom = (long)mHeight; 
		
		WNDCLASS wc;       
		memset(&wc, 0, sizeof(wc));
		

		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc      = (WNDPROC) WndProc;           
		wc.cbClsExtra       = 0;
		wc.cbWndExtra       = 0;
		wc.hInstance        = mInstance;
		wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);    
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);  
		wc.hbrBackground    = (HBRUSH)GetStockObject(BLACK_BRUSH);  
		wc.lpszMenuName     = NULL; 
		wc.lpszClassName    = "nEngine"; 
		
		if (!RegisterClass(&wc)) {
			throw Error("Application", "Cannot register window class");
		}
		
		DWORD dwExStyle, dwStyle;
		if (mFullScreen) {
			DEVMODE dmScreenSettings;  

			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));  

			dmScreenSettings.dmSize			= sizeof(dmScreenSettings);   
			dmScreenSettings.dmPelsWidth    = mWidth;          
			dmScreenSettings.dmPelsHeight   = mHeight;      
			dmScreenSettings.dmBitsPerPel   = mBpp;     
			dmScreenSettings.dmFields		= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) {
				throw Error ("Application", "Cannot enter fullscreen mode");
			}

			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;                 
		} else {
			dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; 
			dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX; 
		}
		
		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

		if (!(mWindowHandle = CreateWindowEx(dwExStyle,           
                "nEngine", mWindowTitle.c_str(),             
                WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,  0, 0, 
				WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top,
                NULL, NULL, mInstance, NULL))) {

			throw Error ("Application", "Cannot initialize a window");
		}

		static  PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),  
			1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
			PFD_TYPE_RGBA, mBpp, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			16, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0  
		};
			
		if (!(mDeviceContext = GetDC(mWindowHandle))) {
			throw Error ("Application", "Cannot create a device context");
		}
		
		GLuint PixelFormat; 	
		if (!(PixelFormat = ChoosePixelFormat(mDeviceContext, &pfd))) {
			throw Error ("Application", "Cannot choose a pixel a format");
		}

		if(!SetPixelFormat(mDeviceContext, PixelFormat, &pfd)) {
			throw Error ("Application", "Cannot set the pixel a format");
		}

		if (!(mGlContext = wglCreateContext(mDeviceContext))) {
			throw Error ("Application", "Cannot create a GL context");
		}
		
		if(!wglMakeCurrent(mDeviceContext, mGlContext)) {
			throw Error ("Application", "Cannot set the GL context");          
		}

		ShowWindow(mWindowHandle, SW_SHOW);    
		SetForegroundWindow(mWindowHandle);   
		SetFocus(mWindowHandle);      
	}
	
	// ------------------------------------------------------------------
	void Application::closeWindow()
	{
		bool shutdownOkay = true;

		if (mFullScreen) {
			ChangeDisplaySettings(NULL, 0);       
		}

		if (mGlContext) {
			if (!wglMakeCurrent(NULL, NULL)) {
				shutdownOkay = false;
			}
			
			if (!wglDeleteContext(mGlContext)) {
				shutdownOkay = false;
			}

			mGlContext = NULL;
		}
		
		if (mDeviceContext && !ReleaseDC(mWindowHandle, mDeviceContext)) {
			mDeviceContext = NULL;
			shutdownOkay = false;
		}
		
		if (mWindowHandle && !DestroyWindow(mWindowHandle)) {
			mWindowHandle = NULL;
			shutdownOkay = false;
		}

		if (!UnregisterClass("nEngine", mInstance)) {
			mInstance = NULL;
			shutdownOkay = false;
		}

		if (!shutdownOkay) {
			throw Error("Application", "Shutdown error");
		}
	}

	// ------------------------------------------------------------------
	void Application::initOpenGL()
	{
		// Init Glew
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			throw Error ("Glew initialisation failed: " + std::string((char*)glewGetErrorString(err)));
		}
		
		if (!glewGetExtension("GL_ARB_fragment_program") || !glewGetExtension("GL_ARB_vertex_program")) {
			throw Error ("OpenGL ARB vertex and fragment shaders are required!");
		}

		// Clear screen
		glClearDepth(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Depth testing
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		// Textures
		glEnable(GL_TEXTURE_2D);

		// Resize the window
		onResize (mWidth, mHeight);
	}
	
	// ------------------------------------------------------------------
	void Application::exit()
	{
		PostMessage(mWindowHandle, WM_QUIT, NULL, NULL);
	}
	
	// ------------------------------------------------------------------
	void Application::drawWaitScreen()
	{
		int boxWidth = 120, boxHeight = 60;
		
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(mWindowHandle, &ps);


		LOGFONT fontDef;
		memset(&fontDef, 0, sizeof(fontDef));
		fontDef.lfHeight = 25;
		fontDef.lfPitchAndFamily = FF_SCRIPT;
		HFONT font = CreateFontIndirect(&fontDef);
		SelectObject(hDC, font);


		RECT rect;
		rect.top = (mHeight - boxHeight) >> 1;
		rect.left = (mWidth - boxWidth) >> 1;
		rect.bottom = rect.top + boxHeight;
		rect.right = rect.left + boxWidth;

		FillRect(hDC, &rect, WHITE_BRUSH);
		TextOut(hDC, rect.left + 15, rect.top + (boxHeight - fontDef.lfHeight) / 2, "Loading...", 10);

		EndPaint(mWindowHandle, &ps);
	}

	// ------------------------------------------------------------------
	void Application::start ()
	{	
		initWindow();
		initOpenGL();
		
		if (mInitFile != "") {
			luaReadFile(mInitFile);
		}

		SoundManager::inst().start(mVolume);
	
		float frameTime = 1000.0f / mMaxFPS;
		mActive = true;
		mDone = false;
		MSG msg;
		
		onSceneInit();

		while (!mDone) {
			float begTime = Timer::inst().getTime();

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					mDone = true;
				} else if (msg.message == NENGINE_RELOAD) {
					drawWaitScreen();
				} else {
					handleMessage(msg);
					TranslateMessage(&msg);	
					DispatchMessage(&msg);
				}
			} else if (mActive) {		
				draw();
				glFlush();
				SwapBuffers(mDeviceContext);
			}

			float endTime = Timer::inst().getTime();

			float remaining = frameTime - (endTime - begTime);
			if (remaining > 0) {
				// Sleep(remaining);
			}
			
			Timer::inst().frameEnd();
		}

		Scene::inst().destroyScene();
		closeLua();
	}
		
	// ------------------------------------------------------------------
	void Application::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity ();
		
		SoundManager::inst().update();

		Scene::inst().update();
		Scene::inst().draw();
		
		GUI::inst().draw();
		Console::inst().draw();
	}
	
	// ------------------------------------------------------------------
	void Application::handleMessage(MSG msg)
	{
		switch (msg.message) {
		case WM_KEYUP:
		{
			unsigned scancode = (msg.lParam >> 16) & 0xFF;
			BYTE keyboardState[256];
			GetKeyboardState(keyboardState);
			char charvalue[2];

			if (ToAsciiEx(msg.wParam, scancode, keyboardState, (LPWORD)&charvalue[0], 0, GetKeyboardLayout(0)) == 1) {
				onKeyUp(0, charvalue[0]);
			} else {
				onKeyUp(msg.wParam, 0);
			}

			break;
		}
		case WM_LBUTTONUP:
		{
			if (!GUI::inst().handleEvent(GUIEvent(GUI_EVENT_MOUSEUP, LOWORD(msg.lParam), HIWORD(msg.lParam)))) {
				onLeftClick(LOWORD(msg.lParam), HIWORD(msg.lParam));
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			if (!GUI::inst().handleEvent(GUIEvent(GUI_EVENT_MOUSEDOWN, LOWORD(msg.lParam), HIWORD(msg.lParam)))) {

			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (!GUI::inst().handleEvent(GUIEvent(GUI_EVENT_MOUSEMOVE, LOWORD(msg.lParam), HIWORD(msg.lParam)))) {
				onMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
			}
			break;
		}
		default:
			return;
		}
	}
		
	// ------------------------------------------------------------------
	void Application::onLeftClick (int x, int y)
	{

	}
	
	// ------------------------------------------------------------------
	void Application::onRightClick(int x, int y)
	{

	}
		
	// ------------------------------------------------------------------
	void Application::onMouseMove(int x, int y)
	{

	}

	// ------------------------------------------------------------------
	void Application::onSceneInit()
	{

	}
	
	// ------------------------------------------------------------------
	void Application::onSceneDestroy()
	{

	}
	
	// ------------------------------------------------------------------
	void Application::onKeyUp(int keyCode, char charCode)
	{
		if (charCode) {
			Console::inst().keyPressed(charCode);
		}

		if (keyCode) {
			Console::inst().specialKeyPressed(keyCode);
		}
	}
	
	// -----------------------------------------------------------------
	std::vector<std::pair<int, int> > Application::getDisplayModes()
	{
		std::vector<std::pair<int, int> > displays;

		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		for(int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm); iModeNum++) {
			bool found = false;
			for (unsigned i = 0; i < displays.size(); ++i) {
				if (displays[i].first == dm.dmPelsWidth && displays[i].second == dm.dmPelsHeight) {
					found = true;
				}
			}

			if (!found) {
				displays.push_back(std::make_pair(dm.dmPelsWidth, dm.dmPelsHeight));
			}
		}

		return displays;
	}
};