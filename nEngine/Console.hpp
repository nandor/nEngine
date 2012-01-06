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
        ~Console();
        Console ();

        void draw();
        void message(int type, std::string str);
        void keyPressed(char key);
        void specialKeyPressed(int key);

        bool isVisible() {
			return visible;
		}
        void setVisible(bool _visible);

        static void luaRegister(lua_State* L);
    private:
        void pushString(int type, std::string str);

        bool visible;
        int height, scrollUp;
        std::string editBuffer;
        static lua_State* luaState;

        std::vector<std::pair<MessageType, std::string> > lines;
        typedef std::vector<std::pair<MessageType, std::string> >::iterator line_iter;
    };
};
#endif /*CONSOLE_HPP*/
