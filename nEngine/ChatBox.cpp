/**
	@file ChatBox.cpp
	@author Licker Nandor

	This file is part of nEngine.
	(c) 2011 Licker Nandor
*/

#include "nHeaders.hpp"
#include "ChatAction.hpp"
#include "ChatMessage.hpp"
#include "Chat.hpp"
#include "GUI.hpp"
#include "ChatBox.hpp"
#include "GUIButton.hpp"
#include "GUILabel.hpp"
#include "GUIButton.hpp"
#include "GUIPanel.hpp"

namespace nEngine {
	template<> ChatBox* ChatBox::Singleton<ChatBox>::__inst = NULL;

	// ------------------------------------------------------------------
	ChatBox::ChatBox()
	{				
		panel = new GUIPanel("chatPanel");
		panel->setSize(GUIMetric(400), GUIMetric(150));
		panel->setAlignment(GUI_ALIGN_CENTER, GUI_ALIGN_BOTTOM);
		panel->setBackgroundColor(Color(0.17f, 0.06f, 0.47f, 0.8f));
		
		lbl = new GUILabel("message");
		lbl->setSize(GUIMetric(90), GUIMetric(30, GUI_METRIC_PERCENT));
		lbl->setAlignment(GUI_ALIGN_CENTER, GUI_ALIGN_NONE);
		lbl->setPosition(GUIMetric(10), GUIMetric(10));
		lbl->setTextColor(Color(1.0f, 1.0f, 1.0f));
		lbl->setText("Hello world!");
		
		GUI::inst().add(panel);
		panel->add(lbl);
		panel->setVisibility(false);
	}


	// ------------------------------------------------------------------
	ChatBox::~ChatBox()
	{

	}


	// ------------------------------------------------------------------
	void ChatBox::setChatMessage(ChatMessage* msg)
	{
		panel->setVisibility(false);

		switch (msg->getType()) {
		case CHAT_PLAYER:
		{
			lbl->setText(msg->getMessage());

			int pos = 20;

			for (int i = 0; i < msg->getNumActions(); ++i) {
				ChatAction& act = msg->getAction(i);

				GUIButton* btn = new GUIButton(msg->getID() + "_act" + boost::lexical_cast<std::string>(i));

				btn->setSize(GUIMetric(90), GUIMetric(25, GUI_METRIC_PERCENT));
				btn->setPosition(GUIMetric(0), GUIMetric(pos += 30));
				btn->setAlignment(GUI_ALIGN_CENTER, GUI_ALIGN_NONE);
				btn->setCaption(act.getText());

				panel->add(btn);
			}

			panel->setSize(GUIMetric(400), GUIMetric(40 + 30 * (msg->getNumActions() + 1)));

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
		panel->setVisibility(false);
	}
};