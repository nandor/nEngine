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
					break;
				default:
					glCallList(ft->getListBase() + text[i]);
					break;
				}
			}
		glPopMatrix();

		Shader::popProgram();
	}

	Vec2 getScreenSize()
	{
		int vp[4];
		glGetIntegerv(GL_VIEWPORT, vp);

		return Vec2(vp[2] - vp[0], vp[3] - vp[1]);
	}

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
}