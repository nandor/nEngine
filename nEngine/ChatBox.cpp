/**
	@file ChatBox.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "ChatBox.hpp"
#include "GUIButton.hpp"

namespace nEngine {
	template<> ChatBox* ChatBox::Singleton<ChatBox>::mInstance = NULL;


	// ------------------------------------------------------------------
	ChatBox::ChatBox()
	{				
		panel = new GUIPanel("chatPanel");
		panel->setSize(Vec2(400, 150));
		panel->setAlignment(GUI_ALIGN_CENTER, GUI_ALIGN_BOTTOM);
		panel->setBackgroundColor(Color(0.17f, 0.06f, 0.47f, 0.8f));
		
		lbl = new GUILabel("message");
		lbl->setSize(Vec2(90, 30), GUI_SIZE_PERCENT);
		lbl->setAlignment(GUI_ALIGN_CENTER, GUI_ALIGN_NONE);
		lbl->setPosition(Vec2(10, 10));
		lbl->setTextColor(Color(1.0f, 1.0f, 1.0f));
		lbl->setText("Hello world!");
		
		GUI::inst().add(panel);
		panel->add(lbl);
		panel->hide();
	}


	// ------------------------------------------------------------------
	ChatBox::~ChatBox()
	{

	}


	// ------------------------------------------------------------------
	void ChatBox::setChatMessage(ChatMessage* msg)
	{
		panel->show();

		switch (msg->getType()) {
		case CHAT_PLAYER:
		{
			lbl->setText(msg->getMessage());

			int pos = 20;

			for (int i = 0; i < msg->getNumActions(); ++i) {
				ChatAction& act = msg->getAction(i);

				GUIButton* btn = new GUIButton(msg->getID() + "_act" + boost::lexical_cast<std::string>(i));

				btn->setSize(Vec2(90, 25), GUI_SIZE_PERCENT, GUI_SIZE_PIXEL);
				btn->setPosition(Vec2(0, pos += 30));
				btn->setAlignment(GUI_ALIGN_CENTER, GUI_ALIGN_NONE);
				btn->setCaption(act.getText());

				panel->add(btn);
			}

			panel->setSize(Vec2(400, 40 + 30 * (msg->getNumActions() + 1)));

			break;
		}
		case CHAT_NPC:
			lbl->setText(msg->getAction(0).getText());
			break;
		}
	}


	// ------------------------------------------------------------------
	void ChatBox::hide()
	{
		panel->hide();
	}
};