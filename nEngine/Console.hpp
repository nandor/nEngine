/**
	@file Console.hpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "Lua.hpp"
#include "Util.hpp"
#include "Font.hpp"
#include "Shader.hpp"
#include "Singleton.hpp"

#define ConsoleError(msg) Console::inst().message(MESSAGE_ERROR, msg);
#define ConsoleWarn(msg)  Console::inst().message(MESSAGE_WARNING, msg);
#define ConsoleLog(msg)   Console::inst().message(MESSAGE_NORMAL, msg);

namespace nEngine {
    enum MessageType {
        MESSAGE_NORMAL      = 1,
        MESSAGE_WARNING     = 2,
        MESSAGE_ERROR       = 3,

        MESSAGE_CODE        = 4
    };

    class Console : public Singleton<Console> {
    public:
        NAPI Console ();
        NAPI ~Console();

        NAPI void draw();
        NAPI void message(int type, std::string str);
        NAPI void keyPressed(char key);
        NAPI void specialKeyPressed(int key);

        bool isVisible() 
		{
			return visible;
		}

        NAPI void setVisible(bool _visible);

        NAPI static void luaRegister(lua_State* L);

    private:

        NAPI void pushString(int type, std::string str);

        bool visible;
        int height, scrollUp;
        std::string editBuffer;
        static lua_State* luaState;

        std::vector<std::pair<MessageType, std::string> > lines;
        typedef std::vector<std::pair<MessageType, std::string> >::iterator line_iter;
    };
};

#endif /*CONSOLE_HPP*/
