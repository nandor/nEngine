/**
	@file nGame.hpp
	@author Licker Nandor

    Main function and entry point of the application.

	(c) 2011 Licker Nandor
*/

#ifndef NGAME_HPP
#define NGAME_HPP

#include "nEngine/GUI.hpp"
#include "nEngine/NPC.hpp"
#include "nEngine/Application.hpp"
#include "Character.hpp"

class nGame : public Application {
public:
	nGame();
	~nGame();
	
private:

	// Engine events
	void onSceneInit();
	void onLuaInit(lua_State* L);
	void onLeftClick(int x, int y);
	void onKeyUp (int keyCode, char charCode);
	void onMouseUp (int mouseX, int mouseY);
	void onMouseMove (int mouseX, int mouseY);

private:
	// GUI events
	void onSettingsClick(GUIEvent& evt);
	void onCancelClick(GUIEvent& evt);
	void onSaveClick(GUIEvent& evt);
	void onExitClick(GUIEvent& evt);
	void onVolumeChanged(GUIEvent& evt);
private:
	// Helpers
	void initUI();
	void beginChat();

private:

	std::string mExamine;
	NPC* mSelectedNPC;
};

#endif /*NGAME_HPP*/