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
	GUILabel* examine = new GUILabel("examineText");
	examine->setFont("gui12");
	examine->setText("Examine: ");
	examine->setTextColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	GUI::inst().add(examine);

	GUIButton* newButton = new GUIButton("newButton");
	newButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
	newButton->setPosition(Vec2(50, 220));
	newButton->setSize(Vec2(150, 45));
	newButton->setFont("gui24");
	newButton->setCaption("New Game");
	newButton->hide();
	GUI::inst().add(newButton);

	GUIButton* loadButton = new GUIButton("loadButton");
	loadButton->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
	loadButton->setPosition(Vec2(50, 160));
	loadButton->setSize(Vec2(150, 45));
	loadButton->setFont("gui24");
	loadButton->setCaption("Load Game");
	loadButton->hide();
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
		
	GUILabel* resLabel = new GUILabel("resLabel") ;
	resLabel->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
	resLabel->setPosition(Vec2(5, 160));
	resLabel->setSize(Vec2(180, 45));
	resLabel->setFont("gui24");
	resLabel->setText("Resolution");
	optionsPanel->add(resLabel);
		
	GUILabel* musicLabel = new GUILabel("musicLabel") ;
	musicLabel->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
	musicLabel->setPosition(Vec2(5, 110));
	musicLabel->setSize(Vec2(180, 45));
	musicLabel->setFont("gui24");
	musicLabel->setText("Music");
	optionsPanel->add(musicLabel);
		
	GUISlider* musicSlider = new GUISlider("musicSlider");
	musicSlider->setAlignment(GUI_ALIGN_LEFT, GUI_ALIGN_BOTTOM);
	musicSlider->setPosition(Vec2(150, 110));
	musicSlider->setSize(Vec2(180, 45));
	musicSlider->setValue(SoundManager::inst().getVolume());
	musicSlider->connect(GUI_EVENT_CHANGED, [musicSlider] (GUIEvent& evt) {
		SoundManager::inst().setVolume(musicSlider->getValue());
	});

	optionsPanel->add(musicSlider);
		
		
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
void nGame::onSettingsClick(GUIEvent& evt)
{
	GUI::inst().get("newButton")->disable();
	GUI::inst().get("loadButton")->disable();
	GUI::inst().get("settingsButton")->disable();
	
	GUI::inst().get("optionsPanel")->show();
}
	
// ------------------------------------------------------------------
void nGame::onCancelClick(GUIEvent& evt)
{
	GUI::inst().get("newButton")->enable();
	GUI::inst().get("loadButton")->enable();
	GUI::inst().get("settingsButton")->enable();
			
	GUI::inst().get("optionsPanel")->hide();
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