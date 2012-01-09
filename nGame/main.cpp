/**
	@file main.cpp
	@author Licker Nandor

    Main function and entry point of the application.

	(c) 2011 Licker Nandor
*/

#include "nEngine/nHeaders.hpp"
#include "nEngine/Application.hpp"
#include "nEngine/GUIButton.hpp"
#include "nEngine/GUICheckbox.hpp"
#include "nEngine/GUIComboBox.hpp"
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
		initUI();
	}
	
	// ------------------------------------------------------------------
	void initUI()
	{
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

		GUIPanel* optionsPanel = new GUIPanel("optionsPanel");
		optionsPanel->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		optionsPanel->setBackgroundColor(Color(0.9f, 0.9f, 0.9f, 0.5f));
		optionsPanel->setPosition(Vec2(240, 45));
		optionsPanel->setSize(Vec2(340, 270));
		optionsPanel->hide();
		GUI::inst().add(optionsPanel);

		GUIButton* saveButton = new GUIButton("saveButton");
		saveButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		saveButton->setPosition(Vec2(5, 5));
		saveButton->setSize(Vec2(150, 45));
		saveButton->setFont("gui24");
		saveButton->setCaption("Save");
		saveButton->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onSaveClick, this, _1));
		optionsPanel->add(saveButton);	

		GUIButton* cancelButton = new GUIButton("cancelButton");
		cancelButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		cancelButton->setPosition(Vec2(185, 5));
		cancelButton->setSize(Vec2(150, 45));
		cancelButton->setFont("gui24");
		cancelButton->setCaption("Cancel");
		cancelButton->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onCancelClick, this, _1));
		optionsPanel->add(cancelButton);	

		GUICheckbox* fullscreenFlag = new GUICheckbox("fullscreenFlag");
		fullscreenFlag->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		fullscreenFlag->setPosition(Vec2(5, 220));
		fullscreenFlag->setSize(Vec2(180, 45));
		fullscreenFlag->setFont("gui24");
		fullscreenFlag->setCaption("Fullscreen");
		fullscreenFlag->setChecked(luaGetGlobalBoolean("fullScreen"));
		optionsPanel->add(fullscreenFlag);

		GUILabel* saveLabel = new GUILabel("saveLabel");
		saveLabel->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		saveLabel->setPosition(Vec2(5, 70));
		saveLabel->setSize(Vec2(300, 45));
		saveLabel->setTextColor(Color(1.0, 0.0, 0.0, 1.0));
		saveLabel->setFont("gui24");
		saveLabel->setText("Save successful!");
		saveLabel->hide();
		optionsPanel->add(saveLabel);

		GUILabel* resLabel = new GUILabel("resLabel");
		resLabel->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		resLabel->setPosition(Vec2(5, 160));
		resLabel->setSize(Vec2(180, 45));
		resLabel->setFont("gui24");
		resLabel->setText("Resolution");
		optionsPanel->add(resLabel);

		GUIComboBox* resBox = new GUIComboBox("resBox");
		resBox->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
		resBox->setPosition(Vec2(150, 165));
		resBox->setSize(Vec2(180, 45));
		resBox->setFont("gui16");

		std::vector<std::pair<int, int> > displayModes = Application::getDisplayModes();

		for (unsigned i = 0; i < displayModes.size(); ++i) {
			resBox->addItem(
				0, 
				boost::lexical_cast<std::string> (displayModes[i].first) + " x " +
				boost::lexical_cast<std::string> (displayModes[i].second)
			);
		}

		optionsPanel->add(resBox);
	}
	
	// ------------------------------------------------------------------
	void onSettingsClick(GUIEvent& evt)
	{
		GUI::inst().get("newButton")->disable();
		GUI::inst().get("loadButton")->disable();
		GUI::inst().get("settingsButton")->disable();
		
		
		GUI::inst().get("optionsPanel")->show();
	}
	
	// ------------------------------------------------------------------
	void onCancelClick(GUIEvent& evt)
	{
		GUI::inst().get("newButton")->enable();
		GUI::inst().get("loadButton")->enable();
		GUI::inst().get("settingsButton")->enable();
		
		GUI::inst().get("optionsPanel")->hide();
		GUI::inst().get("saveLabel")->hide();
	}
	
	// ------------------------------------------------------------------
	void onSaveClick(GUIEvent& evt)
	{		
		GUICheckbox* box = (GUICheckbox*)GUI::inst().get("fullscreenFlag");
		GUIComboBox* resBox = (GUIComboBox*)GUI::inst().get("resBox");
		
		std::vector<std::pair<int, int> > displayModes = Application::getDisplayModes();
		std::pair<int, int> resolution = displayModes[resBox->getSelection()];

		File* file = Resources::inst().require<File> ("fs://data/lua/config.lua");

		std::stringstream ss;

		ss << "--[[\n"
		   << "    Configuration for nEngine\n"
		   << "]]--\n"
		   << "\n"
		   << "fullScreen = " << ((mFullScreen = box->getChecked()) ? "true" : "false") << "\n"
		   << "displayWidth = " << (mWidth = resolution.first) << "\n"
		   << "displayHeight = " << (mHeight = resolution.second) << "\n"
		   << "maxFPS = 40";

		file->write(ss.str());

		GUI::inst().get("saveLabel")->show();

		Timer::inst().queueAction([] (float time) {
			GUI::inst().get("saveLabel")->hide();
		}, 1500.0f);
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
		lua_State* L = luaRegisterEngine();
		luaClass(L, nGame);

		game = new nGame();	
		
		game->start();

		delete game;
    } catch (Error error) {
		MessageBox(NULL, error.getMessage().c_str(), "Error", MB_ICONERROR);
    }

    return 0;
}
