/**
	@file Application.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/
#include "nHeaders.hpp"
#include "Console.hpp"
#include "Timer.hpp"

namespace nEngine {
	template<> Console* Console::Singleton<Console>::mInstance = NULL;

	// ------------------------------------------------------------------
	void errorCallback(const std::string& msg)
	{
		ConsoleError(msg);
	}

	lua_State* Console::luaState = NULL;
	
	// ------------------------------------------------------------------
	Console::Console()
		:visible (false),
		height(10),
		scrollUp(0),
		editBuffer()
	{
	}
	
	// ------------------------------------------------------------------
	Console::~Console()
	{

	}
	
	// ------------------------------------------------------------------
	void Console::draw ()
	{
		if (!visible)
			return;

		Font* ft =  Resources::inst().get<Font>("mono8");
		Font* stats = Resources::inst().get<Font>("mono12");

		int lineHeight = ft == NULL ? 2 : ft->getHeight();
    
		Vec2 sz = getScreenSize();

		int bWidth = sz.getX();
		int bHeight = (lineHeight + 1) * (height + 5);
		
		glPushMatrix();
		glLoadIdentity();
		
		glActiveTextureARB(GL_TEXTURE0_ARB); 
		glDisable(GL_DEPTH_TEST);
		
		Shader::useProgram("color");
		glColor4f(0.7f, 0.7f, 0.7f, 0.7f);

		// Draw the background
		glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(bWidth, 0);
			glVertex2i(bWidth, bHeight);
			glVertex2i(0, bHeight);
		glEnd();

		// Bottom edit box
		glBegin(GL_LINES);
			glVertex2i(bWidth, bHeight - 20);
			glVertex2i(0, bHeight - 20);
		glEnd ();
		
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glPrint(stats, 5, bHeight + 20, "FPS: " + boost::lexical_cast<std::string> ((int)Timer::inst().getFPS()));
		glPrint(stats, 5, bHeight + 34, "Memory: " + boost::lexical_cast<std::string> ((int)Resources::inst().getMemoryUsage() / 1024) + " Kb");
		glPrint (ft, 5, bHeight - 4, "> " + editBuffer);

		int currentLine = max(0, (int)lines.size () - height), pos = 0;
		for (line_iter iter = lines.begin() + currentLine; iter < lines.end (); ++iter)
		{
			std::string prefix = "";
			switch(iter->first)
			{
			case MESSAGE_NORMAL:
				glColor3f(0.0, 0.0, 0.0);
				break;
			case MESSAGE_ERROR:
				glColor3f(1.0, 0.0, 0.0);
				break;
			case MESSAGE_WARNING:
				glColor3f(0.0, 0.0, 1.0);
				break;
			case MESSAGE_CODE:
				prefix = "> ";
				glColor3f(0.0, 0.5, 0.0);
				break;
			}

			glPrint (ft, 5, pos += lineHeight + 3, (prefix + iter->second).c_str());
		}
		
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
	}
	

	// ------------------------------------------------------------------
	void Console::message(int type, std::string str)
	{
		pushString(type, str);
	}

	
	// ------------------------------------------------------------------
	void Console::pushString(int type, std::string str)
	{
		lines.push_back(std::make_pair((MessageType)type, str));
		scrollUp = lines.size () - 1;

		if (type == MESSAGE_CODE)
		{
			if (luaState)
			{
				int error = luaL_loadbuffer(luaState, str.c_str (), str.length(), "console") ||
							lua_pcall(luaState, 0, 0, 0);

				if (error)
				{
					ConsoleError(std::string(lua_tostring(luaState, -1)));
					lua_pop(luaState, 1);
				}
			}
		}
	}
	

	// ------------------------------------------------------------------
	void Console::keyPressed(char key)
	{
		if (!visible && key != '~')
		{
			return;
		}

		switch (key)
		{
		case '~':
			visible = !visible;
			break;
		case '\b':
			if (!editBuffer.empty())
			{
				editBuffer.erase(editBuffer.end () - 1);
			}
			break;
		case 0x0D:
			if (!editBuffer.empty())
			{
				pushString(MESSAGE_CODE, editBuffer);
				editBuffer = "";
			}
			break;
		case '\t':
			break;
		default:
			editBuffer.append (1, key);
			break;
		}
	}

	
	// ------------------------------------------------------------------
	void Console::specialKeyPressed(int key)
	{
		if (!visible)
			return;

		switch (key)
		{
		case Key::Up:
			editBuffer = lines[scrollUp].second;
			if (0 < scrollUp && scrollUp < (int)lines.size ())
			{
				scrollUp--;
			}
			break;
		case Key::Down:
			editBuffer = lines[scrollUp].second;
			if (0 <= scrollUp && scrollUp < (int)lines.size () - 1)
			{
				scrollUp++;
			}
			break;
		default:
			break;
		}
	}

	
	// ------------------------------------------------------------------
	void Console::setVisible(bool v)
	{
		visible = v;
	}
	
	
	// ------------------------------------------------------------------
	luaNewMethod(Console, log)
	{
		const char* str = luaL_checkstring(L, -1);
		Console::inst().message(MESSAGE_NORMAL, std::string(str));
		return 0;
	}
	

	// ------------------------------------------------------------------
	luaNewMethod(Console, warn)
	{
		const char* str = luaL_checkstring(L, -1);
		Console::inst().message(MESSAGE_WARNING, std::string(str));
		return 0;
	}

	
	// ------------------------------------------------------------------
	luaNewMethod(Console, error)
	{
		const char* str = luaL_checkstring(L, -1);
		Console::inst().message(MESSAGE_ERROR, std::string(str));
		return 0;
	}
	

	// ------------------------------------------------------------------
	luaNewMethod(Console, setVisible)
	{
		if (lua_isboolean(L, 1)) {
			bool v = lua_toboolean(L, 1);
			Console::inst().setVisible(v);
		}
		return 0;
	}
	
	luaBeginMeta(Console)
	luaEndMeta()

	luaBeginMethods(Console)
		luaMethod(Console, log)
		luaMethod(Console, error)
		luaMethod(Console, warn)
		luaMethod(Console, setVisible)
	luaEndMethods()

	
	// ------------------------------------------------------------------
	void Console::luaRegister(lua_State* L)
	{
		luaClass(L, Console);

		luaState = L;

		// Replace print with Console.log
		lua_pushcfunction (L, Console_log);
		lua_setglobal(L, "print");

		// Register the error callback
		luaErrorCallback(errorCallback);
	}
};
