/**
	@file Util.cpp
	@author Licker Nandor

    @brief Provides resource management for nEngine

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "Util.hpp"

namespace nEngine 
{
	// ------------------------------------------------------------------
	void glPrint(Font* ft, int x, int y, const std::string& fmt, ...)
	{
		if (ft == NULL) {
			return;
		}

		Shader::useProgram("font");

		char text[256];
		va_list	ap;

		va_start(ap, fmt);
			vsprintf_s(text, fmt.c_str(), ap);
		va_end(ap);

		glPushMatrix();
		glTranslatef(x, y - ft->getHeight(), 0);
		for (int i = 0, len = strlen(text); i < len; ++i) {
			switch (text[i]) {
			case '\t':
				glCallList(ft->getListBase() + (int)' ');
				glCallList(ft->getListBase() + (int)' ');
				glCallList(ft->getListBase() + (int)' ');
				glCallList(ft->getListBase() + (int)' ');
				break;
			default:
				glCallList(ft->getListBase() + text[i]);
				break;
			}
		}
		glPopMatrix();

		Shader::popProgram();
	}
	
	// ------------------------------------------------------------------
	void glPrint(Font* ft, int x, int y, unsigned maxl, const std::string& string)
	{
		std::string toPrint = string;
		unsigned textWidth = ft->getTextWidth(string);
		if (textWidth >= maxl) { 
			unsigned len = 0, length = maxl - 4 * ft->getCharWidth('.');
			textWidth = 0;
			while (textWidth < length && len < string.length()) {
				textWidth += ft->getCharWidth(string[len++]);
			}
				
			toPrint = string.substr(0, len - 1) + "...";
		}

		glPrint(ft, x, y, toPrint);
	}
	
	// ------------------------------------------------------------------
	Vec2 getScreenSize()
	{
		int vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);

		return Vec2(vp[2] - vp[0], vp[3] - vp[1]);
	}
	
	// ------------------------------------------------------------------
	std::string getWin32Error()
	{
		
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			0,
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);

		std::string str((LPTSTR)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return str;
	}
	
	// ------------------------------------------------------------------
	int charToInt(char c)
	{
		char result = 0;
		__asm {
			push eax		
			mov al, c

			cmp al, '0'				; c < '0'
			jl end

			cmp al, '9'				; c <= '9'
			jg notDigit
			sub al, '0'
			mov result, al
			jl end

			notDigit:
			cmp al, 'A'				; c < 'A'
			jl end

			cmp al, 'F'				; c <= 'F'
			jg notUpperChar
			sub al, 55				; 55 = 'A' - 10
			mov result, al
			jl end

			notUpperChar:
			cmp al, 'a'				; c < 'a'
			jl end

			cmp al, 'f'				; c <= 'f'
			jg end

			sub al, 87				; 87 = 'a' - 10
			mov result, al

			end:
			pop eax
		}

		return result;
	}
}