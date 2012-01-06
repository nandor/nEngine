/**
	@file main.cpp
	@author Licker Nandor

    Main function and entry point of the application.

	(c) 2011 Licker Nandor
*/

#include "nEngine/Application.hpp"
#include "Character.hpp"
using namespace nEngine;

class nGame : public Application {
public:
	nGame() : Application()
	{

	}

	~nGame()
	{

	}

	void onSceneInit()
	{
		Scene::inst().setMap("tutorial");
	}

	void initCharacter()
	{
		mCharacter = new Character("character");
		mCharacter->setTile(Vec2(0, 0));

		Scene::inst().addNode(mCharacter);
	}

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

	void onKeyUp (int keyCode, char charCode)
	{
		Application::onKeyUp(keyCode, charCode);
	}

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
