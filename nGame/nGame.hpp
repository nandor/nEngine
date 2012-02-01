/**
	@file nGame.hpp
	@author Licker Nandor

    Main function and entry point of the application.

	(c) 2011 Licker Nandor
*/

#ifndef NGAME_HPP
#define NGAME_HPP

#include "nEngine/GUI.hpp"
#include "nEngine/Application.hpp"
#include "Character.hpp"

class nGame : public Application {
public:
	nGame();
	~nGame();
	
protected:
	
	void initUI();

	void onLuaInit(lua_State* L);
	void onSceneInit();
	void onSettingsClick(GUIEvent& evt);
	void onCancelClick(GUIEvent& evt);
	void onSaveClick(GUIEvent& evt);
	void onLeftClick(int x, int y);
	void onKeyUp (int keyCode, char charCode);
	void onMouseUp (int mouseX, int mouseY);
};

#endif /*NGAME_HPP*/