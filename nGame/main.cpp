/**
	@file main.cpp
	@author Licker Nandor

    Main function and entry point of the application.

	(c) 2011 Licker Nandor
*/

#include "nGame.hpp"
using namespace nEngine;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try {
		nGame myGame;
		myGame.start();

	} catch (Error error) {
		MessageBox(NULL, error.getMessage().c_str(), "Unhandled Exception", MB_ICONERROR);
    }

    return 0;
}
