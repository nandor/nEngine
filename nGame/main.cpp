/**
	@file main.cpp
	@author Licker Nandor

    Main function and entry point of the application.

	(c) 2011 Licker Nandor
*/

#include "nEngine/Application.hpp"
#include "nEngine/GUIButton.hpp"
#include "nEngine/GUICheckbox.hpp"
#include "nEngine/GUI.hpp"
#include "Character.hpp"
using namespace nEngine;

class nGame : public Application {
public:
	// ------------------------------------------------------------------
	nGame() : Application()
	{

	}
	
	// ------------------------------------------------------------------
	~nGame()
	{

	}
	
	// ------------------------------------------------------------------
	void onSceneInit()
	{
		Scene::inst().setMap(Resources::inst().get<Map>("tutorial"));
		

		GUIButton* newButton = new GUIButton("newButton");
		newButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		newButton->setPosition(Vec2(50, 220));
		newButton->setSize(Vec2(150, 45));
		newButton->setFont("gui24");
		newButton->setCaption("New Game");
		GUI::inst().add(newButton);

		
		GUIButton* loadButton = new GUIButton("loadButton");
		loadButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		loadButton->setPosition(Vec2(50, 160));
		loadButton->setSize(Vec2(150, 45));
		loadButton->setFont("gui24");
		loadButton->setCaption("Load Game");
		GUI::inst().add(loadButton);


		GUIButton* settingsButton = new GUIButton("settingsButton");
		settingsButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		settingsButton->setPosition(Vec2(50, 100));
		settingsButton->setSize(Vec2(150, 45));
		settingsButton->setFont("gui24");
		settingsButton->setCaption("Settings");
		settingsButton->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onSettingsClick, this, _1));
		GUI::inst().add(settingsButton);
		
		
		GUIButton* exitButton = new GUIButton("exitButton");
		exitButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		exitButton->setPosition(Vec2(50, 40));
		exitButton->setSize(Vec2(150, 45));
		exitButton->setFont("gui24");
		exitButton->setCaption("Exit");
		exitButton->connect(GUI_EVENT_CLICK, [&] (GUIEvent& evt) -> bool {
			Application::exit();
			return true;
		});
		GUI::inst().add(exitButton);

		GUIButton* saveButton = new GUIButton("saveButton");
		saveButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		saveButton->setPosition(Vec2(210, 40));
		saveButton->setSize(Vec2(150, 45));
		saveButton->setFont("gui24");
		saveButton->setCaption("Save");
		saveButton->hide();
		saveButton->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onSaveClick, this, _1));
		GUI::inst().add(saveButton);	

		GUIButton* cancelButton = new GUIButton("cancelButton");
		cancelButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		cancelButton->setPosition(Vec2(370, 40));
		cancelButton->setSize(Vec2(150, 45));
		cancelButton->setFont("gui24");
		cancelButton->setCaption("Cancel");
		cancelButton->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onCancelClick, this, _1));
		cancelButton->hide();
		GUI::inst().add(cancelButton);	

		GUICheckbox* fullscreenFlag = new GUICheckbox("fullscreenFlag");
		fullscreenFlag->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		fullscreenFlag->setPosition(Vec2(250, 220));
		fullscreenFlag->setSize(Vec2(180, 45));
		fullscreenFlag->setFont("gui24");
		fullscreenFlag->setCaption("Fullscreen");
		fullscreenFlag->hide();
		GUI::inst().add(fullscreenFlag);
	}
	
	// ------------------------------------------------------------------
	void onSettingsClick(GUIEvent& evt)
	{
		GUI::inst().get("newButton")->disable();
		GUI::inst().get("loadButton")->disable();
		GUI::inst().get("settingsButton")->disable();

		GUI::inst().get("saveButton")->show();
		GUI::inst().get("cancelButton")->show();
		GUI::inst().get("fullscreenFlag")->show();
	}
	
	// ------------------------------------------------------------------
	void onCancelClick(GUIEvent& evt)
	{
		GUI::inst().get("newButton")->enable();
		GUI::inst().get("loadButton")->enable();
		GUI::inst().get("settingsButton")->enable();
		
		GUI::inst().get("saveButton")->hide();
		GUI::inst().get("cancelButton")->hide();
		GUI::inst().get("fullscreenFlag")->hide();
	}
	
	// ------------------------------------------------------------------
	void onSaveClick(GUIEvent& evt)
	{
		Resources::inst().free("fs://data/lua/config.lua", RESOURCE_FILE);
	}

	// ------------------------------------------------------------------
	void initCharacter()
	{
		mCharacter = new Character("character");
		mCharacter->setTile(Vec2(0, 0));

		Scene::inst().addNode(mCharacter);
	}
	
	// ------------------------------------------------------------------
	void onLeftClick(int x, int y)
	{
		Vec2 tilePos = Scene::inst().getTileAt(x, y);
		Map* map = Scene::inst().getMap();

		if (map->hasTile (tilePos)) {
			Tile* tile = map->getTile(tilePos);
			Vec2 charPos = mCharacter->getTile();
		
			ChatBox::inst().hide();

			if (MapHelper::neighbours(tilePos, charPos) && tile->hasAction()) {
				tile->useAction();
			} else {
				mCharacter->moveTo (tilePos);
			}
		}
	}
	
	// ------------------------------------------------------------------
	void onKeyUp (int keyCode, char charCode)
	{
		Application::onKeyUp(keyCode, charCode);
	}
	
	// ------------------------------------------------------------------
	void onMouseUp (int mouseX, int mouseY)
	{

	}

private:

	Character* mCharacter;
};

nGame* game = NULL;

luaNewMethod(nGame, initCharacter)
{
	game->initCharacter();
	return 0;
}

luaBeginMeta(nGame)
luaEndMeta()

luaBeginMethods(nGame)
	luaMethod(nGame, initCharacter)
luaEndMethods()

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
    try {
		game = new nGame();
		
		lua_State* L = luaGlobalState();
		luaClass(L, nGame);
		
		game->start();
		
    } catch (Error error) {
		MessageBox(NULL, error.getMessage().c_str(), "Error", MB_ICONERROR);
		if (game) {
			delete game;
		} 
    }

    return 0;
}
