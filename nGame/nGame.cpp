/**
	@file nGame.cpp
	@author Licker Nandor

    Main function and entry point of the application.

	(c) 2011 Licker Nandor
*/

#include "nEngine/nHeaders.hpp"

#include "nEngine/GUI.hpp"
#include "nEngine/GUIPanel.hpp"
#include "nEngine/GUILabel.hpp"
#include "nEngine/GUISlider.hpp"
#include "nEngine/GUIButton.hpp"
#include "nEngine/GUICheckbox.hpp"
#include "nEngine/GUIComboBox.hpp"

#include "nEngine/ChatBox.hpp"
#include "nEngine/SoundManager.hpp"

#include "nGame.hpp"

nGame::nGame() 
		:Application(),
		 mExamine(""),
		 mSelectedNPC(NULL)
{
}
	
// ------------------------------------------------------------------
nGame::~nGame()
{

}
	
// ------------------------------------------------------------------
void nGame::onLuaInit(lua_State* L)
{
	Application::onLuaInit(L);
	luaRegisterCharacter(L);
}

// ------------------------------------------------------------------
void nGame::onSceneInit()
{
	initUI();		
}
	
// ------------------------------------------------------------------
void nGame::initUI()
{
	GUI::inst().loadScript("mainUI");

	Music* background = new Music("zip://data/core.zip/sound/background.ogg");
	SoundManager::inst().setMusic(background);

	((GUIButton*)GUI::inst().get("settingsButton"))->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onSettingsClick, this, _1));
	((GUIButton*)GUI::inst().get("exitButton"))->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onExitClick, this, _1));
	((GUIButton*)GUI::inst().get("saveButton"))->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onSaveClick, this, _1));
	((GUIButton*)GUI::inst().get("cancelButton"))->connect(GUI_EVENT_CLICK, boost::bind(&nGame::onCancelClick, this, _1));
	((GUICheckbox*)GUI::inst().get("fullscreenFlag"))->setChecked(luaGetGlobalBoolean("fullScreen"));
	
	GUISlider* musicSlider = ((GUISlider*)GUI::inst().get("musicSlider"));
	musicSlider->connect(GUI_EVENT_CHANGED, boost::bind(&nGame::onVolumeChanged, this, _1));
	musicSlider->setValue(SoundManager::inst().getVolume());
	
	GUIComboBox* resBox = ((GUIComboBox*)GUI::inst().get("resBox"));
	std::vector<std::pair<int, int> > displayModes = Application::getDisplayModes();

	for (unsigned i = 0; i < displayModes.size(); ++i) {
		resBox->addItem(
			0, 
			boost::lexical_cast<std::string> (displayModes[i].first) + " x " +
			boost::lexical_cast<std::string> (displayModes[i].second)
		);
	}
}

// ------------------------------------------------------------------
void nGame::onVolumeChanged(GUIEvent& evt)
{
	GUISlider* musicSlider = (GUISlider*)GUI::inst().get("musicSlider");
	SoundManager::inst().setVolume(musicSlider->getValue());
}

// ------------------------------------------------------------------
void nGame::onExitClick(GUIEvent& evt)
{
	Application::exit();
}

// ------------------------------------------------------------------
void nGame::onSettingsClick(GUIEvent& evt)
{
	GUI::inst().get("settingsButton")->disable();	
	GUI::inst().get("optionsPanel")->setVisibility(true);
}
	
// ------------------------------------------------------------------
void nGame::onCancelClick(GUIEvent& evt)
{
	GUI::inst().get("settingsButton")->enable();			
	GUI::inst().get("optionsPanel")->setVisibility(false);
}
	
// ------------------------------------------------------------------
void nGame::onSaveClick(GUIEvent& evt)
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
		<< "fullScreen = " << (box->getChecked() ? "true" : "false") << "\n"
		<< "displayWidth = " << resolution.first << "\n"
		<< "displayHeight = " << resolution.second << "\n"
		<< "masterVolume = " << SoundManager::inst().getVolume() << "\n"
		<< "maxFPS = 40";
		
	file->copyData((void*)ss.str().c_str(), ss.str().size());
	file->write();
}
	
// ------------------------------------------------------------------
void nGame::onLeftClick(int mouseX, int mouseY)
{
	Character* character = (Character*)Scene::inst().getNode("character");	
	Map* map = Scene::inst().getMap();

	if (map == NULL) {
		return;
	}
	
	SceneNode* selected = Scene::inst().selectNode(Vec2(mouseX, mouseY));
	
	if (selected != NULL && selected->getType() == "NPC") {
		if (mSelectedNPC) {
			mSelectedNPC->setSelection(false);
		}

		mSelectedNPC = (NPC*)selected;
		mSelectedNPC->setSelection(true);
		return;
	} else {
		if (mSelectedNPC) {
			mSelectedNPC->setSelection(false);
			mSelectedNPC = NULL;
		}
	}

	Vec2 tilePos = Scene::inst().getTileAt(mouseX, mouseY);

	if (map->hasTile (tilePos)) {
		Tile* tile = map->getTile(tilePos);
		Vec2 charPos = character->getTile();
		
		ChatBox::inst().hide();

		if (MapHelper::neighbours(tilePos, charPos) && tile->hasAction()) {
			tile->useAction();
		} else {
			character->moveTo (tilePos);
		}
	}
}
	
// ------------------------------------------------------------------
void nGame::beginChat()
{
	if (mSelectedNPC == NULL) {
		return;
	}
}
	
// ------------------------------------------------------------------
void nGame::onKeyUp (int keyCode, char charCode)
{
	Application::onKeyUp(keyCode, charCode);

	switch (charCode) {
	case 'T': case 't':
		beginChat();
		break;
	}
}
	
// ------------------------------------------------------------------
void nGame::onMouseUp (int mouseX, int mouseY)
{

}

// ------------------------------------------------------------------
void nGame::onMouseMove (int mouseX, int mouseY)
{
	SceneNode* selected = Scene::inst().selectNode(Vec2(mouseX, mouseY));

	mExamine = "";
	if (selected != NULL) {
		mExamine = selected->getID();
	}

	((GUILabel*)GUI::inst().get("examineText"))->setText("Examine: " + mExamine);
}